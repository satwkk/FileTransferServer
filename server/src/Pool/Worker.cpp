#include "Pool/Worker.h"
#include <poll.h>
#include "unistd.h"
#include <Handlers/CommandHandler.h>
#include "Constants.h"
#include "SocketIO.h"

void Worker::Initialize(uint32_t poolIndex)
{
    m_PoolIndex = poolIndex;
    m_Poller.BindDisconnectCallback(std::bind(&Worker::OnClientDisconnected, this, std::placeholders::_1));
    m_Poller.BindRecieveCallback(std::bind(&Worker::OnRecieveCommand, this, std::placeholders::_1, std::placeholders::_2));
    m_Thread = std::thread(&Worker::Update, this);
    std::printf("Worker thread created at index: %d\n", m_PoolIndex);
}

void Worker::Cleanup()
{
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
    std::printf("Worker %d: New client added. Total clients: %d\n", m_PoolIndex, m_ConnectedClients.GetSize());
}

void Worker::OnClientDisconnected(int socketDescriptor)
{
    std::printf("Worker disconnected on fd: %d\n", socketDescriptor);
    close(socketDescriptor);

    m_ConnectedClients.Remove([socketDescriptor](const Client& client) {
        return client.SocketDescriptor == socketDescriptor;
    });

    m_FileDescriptors.Remove([socketDescriptor](const pollfd& fd) {
        return fd.fd == socketDescriptor;
    });
}

void Worker::OnRecieveCommand(int socketDescriptor, const std::string &command)
{
    CommandHandler::Get()->HandleCommand(command, socketDescriptor);
}

void Worker::Update()
{
    while (true)
    {
        m_Poller.Poll(m_FileDescriptors.GetContainer());
    }
}