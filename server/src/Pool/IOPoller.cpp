#include "Pool/IOPoller.h"
#include <unistd.h>
#include "Command.h"
#include "SocketIO.h"

void IOPoller::AddDescriptor(const Client &client)
{
    m_PollDescriptors.push_back({client.SocketDescriptor, POLLIN, 0});
}

void IOPoller::Poll()
{
    int result = poll(m_PollDescriptors.data(), m_PollDescriptors.size(), 2000);
    for (const auto& fd : m_PollDescriptors) 
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
                close(fd.fd);
                RemoveDescriptor(fd.fd);
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

void IOPoller::RemoveDescriptor(int socketDescriptor)
{
    m_PollDescriptors.erase(std::remove_if(
        m_PollDescriptors.begin(),
        m_PollDescriptors.end(),
        [socketDescriptor](const struct pollfd& pfd) {
            return pfd.fd == socketDescriptor;
        }),
        m_PollDescriptors.end());
}