#pragma once

#include <stdint.h>

enum CommandType : uint8_t
{
    NONE = 0x00,
    LIST_DIRECTORY = 0x01
};