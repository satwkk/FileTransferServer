#pragma once

#include "netinet/in.h"
#include <iostream>

struct Client 
{
    int SocketDescriptor = INT_MIN;
    struct sockaddr_in Address;

    Client()
    {
        SocketDescriptor = INT_MIN;
    }

    Client(int socketDescriptor, struct sockaddr_in address)
    : SocketDescriptor(socketDescriptor)
    , Address(address) {}

    ~Client() 
    {
        SocketDescriptor = -1;
        Address = {};
    }

    static Client InvalidClient()
    {
        Client client {};
        return client;
    }

    bool IsValid()
    {
        return SocketDescriptor != INT_MIN;
    }
};