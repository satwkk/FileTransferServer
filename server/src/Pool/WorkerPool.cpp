#include "Pool/WorkerPool.h"
#include "Pool/WorkerPool.h"

WorkerPool::WorkerPool(uint32_t numWorkers)
    : m_NumWorkers(numWorkers)
    , m_Workers(m_NumWorkers)
{
    for (uint32_t i = 0; i < m_NumWorkers; i++) 
    {
        m_Workers[i].Initialize(i);
    }
}

WorkerPool::~WorkerPool()
{
    for (auto& worker : m_Workers) 
    {
        worker.Cleanup();
    }
}

void WorkerPool::AddClient(const Client& client)
{
    // TODO: Optimize worker selection strategy
    auto worker = std::min_element(m_Workers.begin(), m_Workers.end(), [](const Worker& a, const Worker& b) {
        return a.GetNumConnectedClients() < b.GetNumConnectedClients();
    });
    std::printf("[LOG]: Assigning client to worker %d\n", worker->GetPoolIndex());
    worker->OnClientConnected(std::move(client));
}