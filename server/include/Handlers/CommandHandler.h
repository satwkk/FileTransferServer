#pragma once
#include "Command.h"

class CommandHandler 
{
public:
    void HandleCommand(const std::string& name, int fd);
    static CommandHandler* Get();

private:
    std::string GetSafeCommandString(const std::string& input);

private:
    CommandHandler() = default;
    ~CommandHandler() = default;
};