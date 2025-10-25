#pragma once

#include "Command.h"

class ExitCommand : public Command
{
public:
    ExitCommand() = default;
    ~ExitCommand() = default; 
    ExitCommand(CommandType type, int fd);
    void Execute() const override;
};