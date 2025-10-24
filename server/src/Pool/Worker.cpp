#include "Pool/Worker.h"
#include <poll.h>
#include "Handlers/ClientHandler.h"
#include "unistd.h"
#include <Handlers/CommandHandler.h>

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
    for (const auto& client : m_ConnectedClients) 
    {
        close(client.SocketDescriptor);
    }
    m_ConnectedClients.clear();
}

void Worker::AddClient(const Client &client)
{
    m_ConnectedClients.emplace_back(std::move(client));
    m_Poller.AddDescriptor(client);
    ClientHandler::SendMessage(client, "Welcome to the server!\n");
    std::printf("Worker %d: New client added. Total clients: %lu\n", m_PoolIndex, m_ConnectedClients.size());
}

void Worker::OnClientDisconnected(int socketDescriptor)
{
    std::printf("Worker disconnected on fd: %d\n", socketDescriptor);
    m_ConnectedClients.erase(std::remove_if(m_ConnectedClients.begin(), m_ConnectedClients.end(), [socketDescriptor](const Client& client) {
        return client.SocketDescriptor == socketDescriptor;
    }), m_ConnectedClients.end());
}

void Worker::OnRecieveCommand(int socketDescriptor, const std::string &command)
{
    CommandHandler::Get()->HandleCommand(command, socketDescriptor);
}

void Worker::Update()
{
    while (true)
    {
        m_Poller.Poll();
    }

}