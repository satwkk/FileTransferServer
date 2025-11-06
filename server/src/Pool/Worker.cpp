#include "Pool/Worker.h"
#include <poll.h>
#include "unistd.h"
#include <Handlers/CommandHandler.h>
#include <typeinfo>
#include "Constants.h"
#include "SocketIO.h"
#include "Command.h"

void Worker::BindEvents() 
{
    m_Poller.BindDisconnectCallback(std::bind(&Worker::OnClientDisconnected, this, std::placeholders::_1));
    m_Poller.BindRecieveCallback(std::bind(&Worker::OnRecieveCommand, this, std::placeholders::_1, std::placeholders::_2));
}

void Worker::UnbindEvents() 
{
    m_Poller.BindDisconnectCallback(nullptr);
    m_Poller.BindRecieveCallback(nullptr);
}

void Worker::Initialize(uint32_t poolIndex)
{
    BindEvents();
    m_PoolIndex = poolIndex;
    m_Thread = std::thread(&Worker::Update, this);
    iLog << "Worker thread created at index: " << m_PoolIndex << nl;
}

void Worker::Cleanup()
{
    UnbindEvents();
    for (const Client& client : m_ConnectedClients) 
    {
        close(client.SocketDescriptor);
    }
    m_ConnectedClients.Empty();
}

void Worker::OnClientConnected(const Client& client)
{
    {
        std::lock_guard<std::mutex> lock(m_ClientMutex);
        m_ConnectedClients.Add(client);
    }
    {
        std::lock_guard<std::mutex> lock(m_FdMutex);
        m_FileDescriptors.Add({client.SocketDescriptor, POLLIN, 0});
    }
    SocketIO::SendMessage(client, SERVER_WELCOME_MESSAGE);
    iLog << "Worker: " << m_PoolIndex << " New Client added. Total clients: " << m_ConnectedClients.GetSize() << nl;
}

void Worker::OnClientDisconnected(int socketDescriptor)
{
    iLog << "Client disconnected at fd: " << socketDescriptor << nl;
    auto client = GetClientFromDescriptor(socketDescriptor);

    if (client.IsValid()) 
    {
        SocketIO::Close(client);
    }

    {
        std::lock_guard<std::mutex> lock(m_ClientMutex);
        m_ConnectedClients.Remove([socketDescriptor](const Client& client) {
            return client.SocketDescriptor == socketDescriptor;
        });
    }

    {
        std::lock_guard<std::mutex> lock(m_FdMutex);
        m_FileDescriptors.Remove([socketDescriptor](const pollfd& fd) {
            return fd.fd == socketDescriptor;
        });
    }
}

void Worker::OnRecieveCommand(int socketDescriptor, const std::string &command)
{
    auto client = GetClientFromDescriptor(socketDescriptor);
    if (client.IsValid()) 
    {
        m_CommandHandler.HandleCommand(command, client, [&](const std::unique_ptr<Command>& command) {
            if (command->GetType() == CommandType::EXIT) 
            {
                OnClientDisconnected(socketDescriptor);
            }
        });
    }
    else
    {
        // Client is disconnected but somehow sending commands
        // Force disconnect him
        OnClientDisconnected(socketDescriptor);
    }
}

const Client Worker::GetClientFromDescriptor(int socketDescriptor)
{
    std::lock_guard<std::mutex> lock(m_ClientMutex);
    auto client = std::find_if(m_ConnectedClients.begin(), m_ConnectedClients.end(), [socketDescriptor](const Client& client) {
        return client.SocketDescriptor == socketDescriptor;
    });
    if (client != m_ConnectedClients.end()) 
    {
        return *client;
    }
    return Client::InvalidClient();
}

void Worker::Update()
{
    while (true)
    {
        std::vector<pollfd> fdsCopy;
        {
            std::lock_guard<std::mutex> lock(m_FdMutex);
            fdsCopy = m_FileDescriptors.GetContainer();
        }
        m_Poller.Poll(fdsCopy);
    }
}
