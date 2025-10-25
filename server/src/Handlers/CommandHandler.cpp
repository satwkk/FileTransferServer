#include "Handlers/CommandHandler.h"
#include <functional>
#include <sys/socket.h>
#include "SocketIO.h"

static CommandHandler* s_Instance = nullptr;

void CommandHandler::HandleCommand(const std::string& name, int fd)
{
    std::string safeCommand = GetSafeCommandString(name);
    std::unique_ptr<Command> command = Command::Create(safeCommand, fd);
    if (command)
    {
        command->Execute([&](int fd, const std::string& response) {
            SocketIO::SendMessage(fd, response);
        });
    }
    else 
    {
        std::printf("Unknown command\n");
    }
}

std::string CommandHandler::GetSafeCommandString(const std::string &input)
{
    std::string safeString = input;
    safeString.erase(safeString.find_last_not_of("\n\r") + 1);
    return safeString;
}

CommandHandler *CommandHandler::Get()
{
    if (s_Instance == nullptr)
    {
        s_Instance = new CommandHandler();
    }
    return s_Instance;
}