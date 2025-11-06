#pragma once
#include <iostream>
#include "Logging/Logger.h"

#define SERVER_PORT 9001

#define Check(cond, message) \
if (!(cond)) { \
    eLog << message << nl; \
    exit(EXIT_FAILURE); \
}

#define Assert(cond, message) \
if (!(cond)) { \
    eLog << message << nl; \
}
