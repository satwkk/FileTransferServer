#pragma once

#include "Worker.h"
#include <vector>

class WorkerPool 
{
public:
    WorkerPool() = default;
    WorkerPool(uint32_t numWorkers);
    ~WorkerPool();
    WorkerPool(const WorkerPool&) = delete;
    WorkerPool& operator=(const WorkerPool&) = delete;

    void AddClient(const Client& client);

private:
    uint32_t m_NumWorkers;
    std::vector<Worker> m_Workers;
};