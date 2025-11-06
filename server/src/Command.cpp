#include "Command.h"
#include "Commands/ListDirectoryCommand.h"
#include "Commands/ExitCommand.h"
#include "Commands/DownloadCommand.h"
#include "Commands/PutFileCommand.h"
#include <unordered_map>

static std::unordered_map<std::string, CommandType> commandMap = {
    {"ls", LIST_DIRECTORY},
    {"download", DOWNLOAD},
    {"put", PUT},
    {"exit", EXIT}
};

Command::Command(CommandType type, const Client& client)
    : m_Type(type)
    , m_InvokerClient(client)
{
}

std::unique_ptr<Command> Command::Create(const std::string& name, const std::vector<std::string>& args, const Client& client)
{
    CommandType type = GetCommandTypeFromName(name);
    switch (type)
    {
        case LIST_DIRECTORY: 
        {
            return std::make_unique<ListDirectoryCommand>(type, client);
        }
        case EXIT:
        {
            return std::make_unique<ExitCommand>(type, client);
        }
        case DOWNLOAD: 
        {
            return std::make_unique<DownloadCommand>(type, args, client);
        }
        case PUT: 
        {
            return std::make_unique<PutFileCommand>(type, args, client);
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