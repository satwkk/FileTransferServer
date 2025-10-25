#include "Commands/ListDirectoryCommand.h"
#include <filesystem>
#include <sys/socket.h>
#include "SocketIO.h"

ListDirectoryCommand::ListDirectoryCommand(CommandType type, int fd)
    : Command::Command(type, fd)
{
}

void ListDirectoryCommand::Execute() const
{
    std::string path = ".";
    std::string response = "Directory listing for " + path + ":\n";

    for (const auto& entry : std::filesystem::directory_iterator(path)) 
    {
        response += entry.path().filename().string() + "\n";
    }

    SocketIO::SendMessage(m_InvokerFd, response);
}