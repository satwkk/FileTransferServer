#pragma once

#include <vector>
#include <thread>
#include <poll.h>
#include "Client.h"
#include "IOPoller.h"
#include "Containers/EasyArray.h"

class Worker 
{
public:
    Worker() = default;
    ~Worker() = default;

    void Initialize(uint32_t poolIndex);
    void Cleanup();
    void Update();
    void AddClient(const Client& client);

    uint32_t GetNumConnectedClients() const { return m_ConnectedClients.GetSize(); }
    uint32_t GetPoolIndex() const { return m_PoolIndex; }

private:
    void OnClientDisconnected(int socketDescriptor);
    void OnRecieveCommand(int socketDescriptor, const std::string& command);

private:
    std::thread m_Thread;
    uint32_t m_PoolIndex;
    EasyArray<Client> m_ConnectedClients;
    EasyArray<struct pollfd> m_FileDescriptors;
    IOPoller m_Poller;
};