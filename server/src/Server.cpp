#include "Server.h"
#include <iostream>
#include "Defines.h"
#include "Constants.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

Server::Server(const std::string& host, int port)
    : m_Port(port)
    , m_Host(host)
    , m_IsRunning(false)
    , m_WorkerPool(5)
{
    m_SocketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    Check(m_SocketDescriptor != -1, "Failed to create socket");

    struct sockaddr_in serverAddress {};
    serverAddress.sin_port = htons(m_Port);
    serverAddress.sin_addr.s_addr = inet_addr(m_Host.c_str());
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_len = sizeof(serverAddress);
    
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

void Server::Run()
{
    std::printf("Server is running on %s:%d\n", m_Host.c_str(), m_Port);

    while (m_IsRunning) 
    {
        struct sockaddr_in clientSocket {};
        socklen_t clientSocketLen = sizeof(clientSocket);
        int clientSocketDescriptor = accept(m_SocketDescriptor, reinterpret_cast<sockaddr*>(&clientSocket), &clientSocketLen);
        Assert(clientSocketDescriptor != -1, "Failed to accept client connection");

        std::printf("[SUCCESS]: Client connected from %s:%d\n", inet_ntoa(clientSocket.sin_addr), ntohs(clientSocket.sin_port));
        m_WorkerPool.AddClient(Client(clientSocketDescriptor, clientSocket));
    }
}