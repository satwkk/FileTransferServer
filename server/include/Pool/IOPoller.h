#pragma once

#include <poll.h>
#include <functional>
#include <vector>
#include "Client.h"

class IOPoller
{
public:
    IOPoller() = default;
    ~IOPoller() = default;

    // void AddDescriptor(const Client& client);
    void Poll(std::vector<struct pollfd>& descriptors);
    void BindRecieveCallback(std::function<void(int, const std::string&)> callback);
    void BindDisconnectCallback(std::function<void(int)> callback);

private:
    std::function<void(int)> m_DisconnectCallback;
    std::function<void(int, const std::string&)> m_RecieveCallback;
};