#pragma once

#include <stdint.h>

enum CommandType : uint16_t
{
    NONE = 0x0000,
    LIST_DIRECTORY = 0x0001,
    EXIT = 0x0100,
    DOWNLOAD = 0x1000,
    PUT = 0x0011,
};