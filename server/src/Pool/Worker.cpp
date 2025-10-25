#include "Pool/Worker.h"
#include <poll.h>
#include "unistd.h"
#include <Handlers/CommandHandler.h>
#include "Constants.h"
#include "SocketIO.h"
#include "Event/EventBus.h"

void Worker::BindEvents() 
{
    m_Poller.BindDisconnectCallback(std::bind(&Worker::OnClientDisconnected, this, std::placeholders::_1));
    m_Poller.BindRecieveCallback(std::bind(&Worker::OnRecieveCommand, this, std::placeholders::_1, std::placeholders::_2));
}

void Worker::UnbindEvents() 
{
}

void Worker::Initialize(uint32_t poolIndex)
{
    BindEvents();
    m_PoolIndex = poolIndex;
    m_Thread = std::thread(&Worker::Update, this);
    std::printf("[LOG]: Worker thread created at index: %d\n", m_PoolIndex);
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

void Worker::AddClient(const Client &client)
{
    m_ConnectedClients.Add(client);
    m_FileDescriptors.Add({client.SocketDescriptor, POLLIN, 0});
    SocketIO::SendMessage(client.SocketDescriptor, SERVER_WELCOME_MESSAGE);
    std::printf("[LOG]: Worker %d: New client added. Total clients: %d\n", m_PoolIndex, m_ConnectedClients.GetSize());
}

void Worker::OnClientDisconnected(int socketDescriptor)
{
    std::printf("[LOG]: Worker disconnected on fd: %d\n", socketDescriptor);
    SocketIO::Close(socketDescriptor);

    m_ConnectedClients.Remove([socketDescriptor](const Client& client) {
        return client.SocketDescriptor == socketDescriptor;
    });

    m_FileDescriptors.Remove([socketDescriptor](const pollfd& fd) {
        return fd.fd == socketDescriptor;
    });
}

void Worker::OnRecieveCommand(int socketDescriptor, const std::string &command)
{
    m_CommandHandler.HandleCommand(command, socketDescriptor, [&](const std::unique_ptr<Command>& command) {
        if (command->GetType() == CommandType::EXIT) 
        {
            OnClientDisconnected(socketDescriptor);
        }
    });
}

void Worker::Update()
{
    while (true)
    {
        m_Poller.Poll(m_FileDescriptors.GetContainer());
    }
}