#pragma once
#include <vector>

#include "Client.h"
#include "Pool/WorkerPool.h"

struct ServerContext 
{
};

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
    std::string m_Host;
    int m_Port;
    int m_SocketDescriptor;
    bool m_IsRunning;
    std::vector<Client> m_ClientHandles;
    WorkerPool m_WorkerPool;
    ServerContext m_Context;
    int m_bReuseAddress;
};