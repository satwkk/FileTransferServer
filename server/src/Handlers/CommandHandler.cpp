#include "Handlers/CommandHandler.h"
#include <functional>
#include <sys/socket.h>
#include "SocketIO.h"
#include <vector>
#include "Client.h"
#include <unordered_set>

static std::unordered_set<char> s_BlacklistCharacters = {'\r', '\n', ';', '|', '&' };

CommandHandler::CommandHandler() 
{
}

void CommandHandler::HandleCommand(const std::string& name, const Client& client, const std::function<void(const std::unique_ptr<Command>& command)>& onComplete) 
{
    std::string safeCommand = GetSafeCommandString(name);
    std::vector<std::string> tokens = Tokenize(safeCommand);
    if (tokens.empty()) return;

    std::string cmd = tokens.front();
    auto it = tokens.begin(); it++;
    std::vector<std::string> args = std::vector<std::string>(it, tokens.end());
    std::unique_ptr<Command> command = Command::Create(cmd, args, client);
    if (command)
    {
        command->Execute();
        onComplete(command);
    }
    else 
    {
        SocketIO::SendMessage(client, "Unknown command\n");
        wLog << "Unknown command from client: " << client.SocketDescriptor << nl;
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
    auto it = std::remove_if(safeString.begin(), safeString.end(), [](const char ch) {
        return s_BlacklistCharacters.contains(ch);
    });
    safeString.erase(it, safeString.end());
    return safeString;
}
