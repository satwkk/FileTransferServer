#include "Pool/IOPoller.h"
#include <unistd.h>
#include "Command.h"
#include "SocketIO.h"

void IOPoller::Poll(std::vector<struct pollfd>& descriptors)
{
    int _ = poll(descriptors.data(), descriptors.size(), 1000);
    for (const auto& desc : descriptors) 
    {
        if (desc.revents & POLLIN)
        {
            std::string message = SocketIO::RecieveMessageRaw(desc.fd);
            if (message == "__no_data__")
            {
                continue;
            }
            if (message.empty())
            {
                m_DisconnectCallback(desc.fd);
            }
            else 
            {
                m_RecieveCallback(desc.fd, message);
            }
        }
    }
}

void IOPoller::BindRecieveCallback(std::function<void(int, const std::string &)> callback)
{
    m_RecieveCallback = callback;
}

void IOPoller::BindDisconnectCallback(std::function<void(int)> callback)
{
    m_DisconnectCallback = callback;
}
