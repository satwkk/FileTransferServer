#pragma once

#include <string>

struct ClientDisconnectEvent
{
    int ClientDescriptor;
};

struct MessageReceiveEvent
{
    int ClientDescriptor;
    std::string Message;
};