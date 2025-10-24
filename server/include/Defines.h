#pragma once
#include <iostream>

#define SERVER_PORT 9001

#define Check(cond, message) \
if (!(cond)) { \
    std::printf("[ERROR]: %s\n", message); \
    exit(EXIT_FAILURE); \
}

#define Assert(cond, message) \
if (!(cond)) { \
    std::printf("[ERROR]: %s\n", message); \
}
