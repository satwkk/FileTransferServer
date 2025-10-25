#include "Pool/IOPoller.h"
#include <unistd.h>
#include "Command.h"
#include "SocketIO.h"

void IOPoller::Poll(std::vector<struct pollfd>& descriptors)
{
    int result = poll(descriptors.data(), descriptors.size(), 1000);
    for (const auto& fd : descriptors) 
    {
        if (fd.revents & POLLIN)
        {
            // TODO: Decide max message size
            std::string message = SocketIO::RecieveMessageRaw(fd.fd);
            if (message == "__no_data__")
            {
                continue;
            }
            if (message.empty())
            {
                m_DisconnectCallback(fd.fd);
            }
            else 
            {
                m_RecieveCallback(fd.fd, message);
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
