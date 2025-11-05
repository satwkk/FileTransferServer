#pragma once
#include "Command.h"

#include <functional>

class ListDirectoryCommand : public Command 
{
public:
    ListDirectoryCommand() = default;
    ListDirectoryCommand(CommandType type, const Client& client);
    void Execute() const override;
};