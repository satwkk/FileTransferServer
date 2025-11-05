#include "Commands/ListDirectoryCommand.h"
#include <filesystem>
#include <sys/socket.h>
#include "SocketIO.h"

ListDirectoryCommand::ListDirectoryCommand(CommandType type, const Client& client)
    : Command::Command(type, client)
{
}

void ListDirectoryCommand::Execute() const
{
    std::string path = ".";
    std::string response {};

    for (const auto& entry : std::filesystem::directory_iterator(path)) 
    {
        response += entry.path().filename().string() + "\n";
    }

    SocketIO::SendMessage(GetInvoker(), response);
}