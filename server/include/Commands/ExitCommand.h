#pragma once

#include "Command.h"

class ExitCommand : public Command
{
public:
    ExitCommand() = default;
    ~ExitCommand() = default; 
    ExitCommand(CommandType type, const Client& client);
    void Execute() const override;
};