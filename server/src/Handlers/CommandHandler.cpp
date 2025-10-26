#include "Handlers/CommandHandler.h"
#include <functional>
#include <sys/socket.h>
#include "SocketIO.h"
#include "Event/EventBus.h"
#include <numeric>
#include <vector>

CommandHandler::CommandHandler() 
{
}

void CommandHandler::HandleCommand(const std::string& name, int fd, const std::function<void(const std::unique_ptr<Command>& command)>& onComplete)
{
    std::string safeCommand = GetSafeCommandString(name);
    std::vector<std::string> tokens = Tokenize(safeCommand);
    if (tokens.empty()) return;

    std::string cmd = tokens.front();
    auto it = tokens.begin(); it++;
    std::vector<std::string> args = std::vector<std::string>(it, tokens.end());
    std::unique_ptr<Command> command = Command::Create(cmd, args, fd);
    if (command)
    {
        command->Execute();
        onComplete(command);
    }
    else 
    {
        SocketIO::SendMessage(fd, "Unknown command\n");
        std::printf("[WARN]: Unknown command\n");
    }
}

std::vector<std::string> CommandHandler::Tokenize(const std::string &rawString)
{
    std::vector<std::string> tokens {};
    std::string token {};
    for (char ch : rawString) 
    {
        if (ch == ' ') 
        {
            tokens.push_back(token);
            token = "";
        }
        else
        {
            token += ch;
        }
    }
    if (token.size() > 0) 
    {
        tokens.push_back(token);
    }
    return tokens;
}

std::string CommandHandler::GetSafeCommandString(const std::string &input)
{
    std::string safeString = input;
    safeString.erase(safeString.find_last_not_of("\n\r") + 1);
    return safeString;
}