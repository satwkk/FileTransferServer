#pragma once
#include "Command.h"
#include <functional>

class CommandHandler
{
public:
    CommandHandler();
    ~CommandHandler() = default;
    void HandleCommand(const std::string& name, int fd, const std::function<void(const std::unique_ptr<Command>& command)>& onComplete);
    std::vector<std::string> Tokenize(const std::string& rawString);

private:
    std::string GetSafeCommandString(const std::string& input);
};