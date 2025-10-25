#include "Pool/IOPoller.h"
#include <unistd.h>
#include "Command.h"
#include "SocketIO.h"

void IOPoller::Poll(std::vector<struct pollfd>& descriptors)
{
    int result = poll(descriptors.data(), descriptors.size(), 2000);
    for (const auto& fd : descriptors) 
    {
        if (fd.revents & POLLIN)
        {
            // TODO: Decide max message size
            std::string message = SocketIO::RecieveMessageRaw(fd.fd);
            if (message.size() > 0) 
            {
                m_RecieveCallback(fd.fd, message);
            }
            else if ((message.size() == 0) || (message.size() < 0 && (errno != EWOULDBLOCK && errno != EAGAIN))) 
            {
                m_DisconnectCallback(fd.fd);
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