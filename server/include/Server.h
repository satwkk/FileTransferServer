#pragma once
#include <vector>

#include "Client.h"
#include "Pool/WorkerPool.h"

class Server 
{
public:
    Server() = default;
    Server(const std::string& host, int port);
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    ~Server();

public:
    void Run();

private:
    void HandleClient(const Client& client);

private:
    std::string m_Host;
    int m_Port;
    int m_SocketDescriptor;
    bool m_IsRunning;
    std::vector<Client> m_ClientHandles;
    WorkerPool m_WorkerPool;
};