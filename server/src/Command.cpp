#include "Command.h"
#include "Commands/ListDirectoryCommand.h"
#include <unordered_map>

static std::unordered_map<std::string, CommandType> commandMap = {
    {"ls", LIST_DIRECTORY}
};

Command::Command(CommandType type, int fd)
    : m_Type(type)
    , m_InvokerFd(fd)
{
}

std::unique_ptr<Command> Command::Create(const std::string& name, int fd)
{
    CommandType type = GetCommandTypeFromName(name);
    switch (type)
    {
        case LIST_DIRECTORY: 
        {
            return std::make_unique<ListDirectoryCommand>(type, fd);
        }
        default: 
        {
            return nullptr;
        }
    }
}

CommandType Command::GetCommandTypeFromName(const std::string &name)
{
    return commandMap.contains(name) ? commandMap[name] : CommandType::NONE;
}

void Command::Execute() const
{
}
