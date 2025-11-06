#include "Server.h"
#include "Defines.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <Event/EventBus.h>
#include <Event/Events.h>

static Server* s_Instance = nullptr;

Server::Server(const std::string& host, int port)
    : m_Port(port)
    , m_Host(host)
    , m_IsRunning(false)
    , m_WorkerPool(5)
    , m_bReuseAddress(1)
{
    s_Instance = this;

    m_SocketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    Check(m_SocketDescriptor != -1, "Failed to create socket");

    struct sockaddr_in serverAddress {};
    serverAddress.sin_port = htons(m_Port);
    serverAddress.sin_addr.s_addr = inet_addr(m_Host.c_str());
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_len = sizeof(serverAddress);

    setsockopt(m_SocketDescriptor, SOL_SOCKET, SO_REUSEADDR, &m_bReuseAddress, sizeof(int));

    int bindRes = bind(m_SocketDescriptor, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress));
    Check(bindRes != -1, "Failed to bind socket");

    int listenRes = listen(m_SocketDescriptor, 5);
    Check(listenRes != -1, "Failed to listen on socket");

    m_IsRunning = true;
}

Server::~Server()
{
    close(m_SocketDescriptor);
}

Server* Server::Get()
{
    return s_Instance;
}

void Server::Run()
{
    iLog << "Server is running on " << m_Host << ":" << m_Port << nl;

    while (m_IsRunning) 
    {
        struct sockaddr_in clientSocket {};
        socklen_t clientSocketLen = sizeof(clientSocket);
        int clientSocketDescriptor = accept(m_SocketDescriptor, reinterpret_cast<sockaddr*>(&clientSocket), &clientSocketLen);
        Assert(clientSocketDescriptor != -1, "Failed to accept client connection");

        iLog << "Client connected from " << std::string(inet_ntoa(clientSocket.sin_addr)) << ":" << ntohs(clientSocket.sin_port) << nl;
        m_WorkerPool.AddClient(Client(clientSocketDescriptor, clientSocket));
    }
}
