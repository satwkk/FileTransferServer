#include "Handlers/CommandHandler.h"
#include <functional>
#include <sys/socket.h>
#include "SocketIO.h"
#include "Event/EventBus.h"

CommandHandler::CommandHandler() 
{
}

void CommandHandler::HandleCommand(const std::string& name, int fd, const std::function<void(const std::unique_ptr<Command>& command)>& onComplete)
{
    std::string safeCommand = GetSafeCommandString(name);
    std::unique_ptr<Command> command = Command::Create(safeCommand, fd);
    if (command)
    {
        command->Execute([&](int fd, const std::string& response) {
            if (response.size() > 0)
            {
                SocketIO::SendMessage(fd, response);
            }
            onComplete(command);
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