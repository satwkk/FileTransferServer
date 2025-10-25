#pragma once
#include "Command.h"

#include <functional>

class ListDirectoryCommand : public Command 
{
public:
    ListDirectoryCommand() = default;
    ListDirectoryCommand(CommandType type, int fd);
    void Execute() const override;
};