#pragma once

#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include "Client.h"
#include "Logging/Logger.h"
namespace SocketIO 
{
    static std::string GetSafeMessage(const std::string& rawMessage)
    {
        std::string safeMessage = rawMessage;
        safeMessage.erase(safeMessage.find_last_not_of("\n\r")+1);
        return safeMessage;
    }

    static void SendMessage(const Client& client, const std::string& message)
    {
        try 
        {
            send(client.SocketDescriptor, message.data(), message.size(), 0);
        }
        catch (std::exception& e) 
        {
            eLog << "Error " << e.what() << " while SendMessage on FD: " << client.SocketDescriptor << nl;
        }
    }

    static std::string RecieveMessageRaw(int socketDescriptor, uint32_t recvSize = 1024)
    {
        std::string response {};
        char buffer[1024];
        int bytesRead = recv(socketDescriptor, buffer, sizeof(buffer), 0);
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
                eLog << "recv() failed on FD: " << socketDescriptor << ", errno: " << errno << nl;
                return "";
            }
        }
    }

    static std::string RecieveMessage(const Client& client, uint32_t recvSize = 1024)
    {
        return GetSafeMessage(RecieveMessageRaw(client.SocketDescriptor, recvSize));
    }

    static void Close(const Client& client)
    {
        close(client.SocketDescriptor);
    }
}