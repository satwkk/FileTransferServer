#pragma once

#include "netinet/in.h"
#include <iostream>

struct Client 
{
    int SocketDescriptor;
    struct sockaddr_in Address;

    Client(int socketDescriptor, struct sockaddr_in address)
    : SocketDescriptor(socketDescriptor)
    , Address(address) {}

    ~Client() 
    {
        SocketDescriptor = -1;
        Address = {};
    }
};