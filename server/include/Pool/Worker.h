#pragma once

#include <thread>
#include <poll.h>
#include "Client.h"
#include "IOPoller.h"
#include "Containers/EasyArray.h"
#include <Event/Events.h>
#include <Handlers/CommandHandler.h>
#include <mutex>
#include <optional>
#include <unordered_map>

class Worker 
{
public:
    Worker() = default;
    ~Worker() = default;

    void Initialize(uint32_t poolIndex);
    void Cleanup();
    void Update();

    inline uint32_t GetNumConnectedClients() const { return m_ClientMap.size(); }
    inline uint32_t GetPoolIndex() const { return m_PoolIndex; }

    void OnClientConnected(const Client& client);

private:
    void BindEvents();
    void UnbindEvents();
    void OnClientDisconnected(int socketDescriptor);
    void OnRecieveCommand(int socketDescriptor, const std::string& command);
    const Client GetClientFromDescriptor(int socketDescriptor);

private:
    std::thread m_Thread;
    uint32_t m_PoolIndex;
    std::unordered_map<int, Client> m_ClientMap;
    EasyArray<struct pollfd> m_FileDescriptors;
    CommandHandler m_CommandHandler;
    IOPoller m_Poller;
    std::mutex m_ClientMutex;
    std::mutex m_FdMutex;
};
