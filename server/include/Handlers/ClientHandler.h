#pragma once

#include "Client.h"
#include <string>
#include "Command.h"

namespace ClientHandler 
{
    static void SendMessage(const Client& client, const std::string& message) 
    {
        send(client.SocketDescriptor, message.c_str(), message.size(), 0);
    }

    static void HandleClient(const Command& command, int fd)
    {
    }
}