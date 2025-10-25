#pragma once

#include <string>
#include <sys/socket.h>
#include <unistd.h>

namespace SocketIO 
{
    static std::string GetSafeMessage(const std::string& rawMessage)
    {
        std::string safeMessage = rawMessage;
        safeMessage.erase(safeMessage.find_last_not_of("\n\r")+1);
        return safeMessage;
    }

    static void SendMessage(int fd, const std::string& message)
    {
        try 
        {
            send(fd, message.data(), message.size(), 0);
        }
        catch (std::exception& e) 
        {
            std::printf("[ERROR]: Error %s while SendMessage on FD: %d\n", e.what(), fd);
        }
    }

    static std::string RecieveMessageRaw(int fd, uint32_t recvSize = 1024)
    {
        std::string response {};
        char buffer[1024];
        int bytesRead = recv(fd, buffer, sizeof(buffer), 0);
        if (bytesRead > 0) 
        {
            return std::string(buffer, bytesRead);
        }
        else if (bytesRead == 0) 
        {
            return "";
        }
        else 
        {
            if (errno == EWOULDBLOCK || errno == EAGAIN) 
            {
                return "__no_data__";
            }
            else 
            {
                std::printf("[ERROR]: recv() failed on FD: %d, errono: %d\n", fd, errno);
                return "";
            }
        }
    }

    static std::string RecieveMessage(int fd, uint32_t recvSize = 1024)
    {
        return GetSafeMessage(RecieveMessageRaw(fd, recvSize));
    }

    static void Close(int fd)
    {
        close(fd);
    }
}