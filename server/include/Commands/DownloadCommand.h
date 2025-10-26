#pragma once

#include "Command.h"

class DownloadCommand : public Command 
{
public:
    DownloadCommand() = default;
    DownloadCommand(CommandType type, const std::vector<std::string>& args, int fd);

    void Execute() const override;

private:
    std::vector<std::string> m_Args;
};