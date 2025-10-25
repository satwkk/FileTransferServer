#pragma once

#include "Command.h"

class ExitCommand : public Command
{
public:
    ExitCommand() = default;
    ~ExitCommand() = default; 
    ExitCommand(CommandType type, int fd);
    void Execute(std::function<void(int fd, const std::string& response)> onExecuteComplete) const override;
};