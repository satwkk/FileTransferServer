#pragma once

#include <string>
#include <sys/socket.h>

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
        try 
        {
            char buffer[1024];
            int bytesRead = recv(fd, buffer, sizeof(buffer), 0);
            if (bytesRead > 0) 
            {
                response = std::string(buffer, bytesRead);
            }
            return response;
        }
        catch (std::exception& e) 
        {
            std::printf("[ERROR]: Error %s while RecieveMessage on FD: %d\n", e.what(), fd);
        }
        return std::string{};
    }

    static std::string RecieveMessage(int fd, uint32_t recvSize = 1024)
    {
        return GetSafeMessage(RecieveMessageRaw(fd, recvSize));
    }
}