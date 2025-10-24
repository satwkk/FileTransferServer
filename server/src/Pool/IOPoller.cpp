#include "Pool/IOPoller.h"
#include <unistd.h>
#include "Command.h"

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
            std::vector<char> buf(1024);
            int bytesRead = recv(fd.fd, buf.data(), buf.size(), 0);
            if (bytesRead > 0) 
            {
                m_RecieveCallback(fd.fd, std::string(buf.data(), bytesRead));
            }
            else if ((bytesRead == 0) || (bytesRead < 0 && (errno != EWOULDBLOCK && errno != EAGAIN))) 
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