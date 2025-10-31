#pragma once

#include "Command.h"

class PutFileCommand : public Command 
{
public:
    PutFileCommand() = default;
    PutFileCommand(CommandType type, const std::vector<std::string>& args, int fd);

    void Execute() const override;

private:
    std::vector<std::string> m_Args;
};
