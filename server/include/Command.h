#pragma once

#include <string>
#include <memory>
#include "Enums.h"
#include <functional>
#include "Client.h"

class Command 
{
public:
    static std::unique_ptr<Command> Create(const std::string& name, const std::vector<std::string>& args, const Client& client);
    static CommandType GetCommandTypeFromName(const std::string& name);
    virtual ~Command() = default;

    virtual void Execute() const = 0;

    const CommandType GetType() const { return m_Type; }
    const Client& GetInvoker() const { return m_InvokerClient; }

protected:
    Command() = default;
    Command(CommandType type, const Client& client);

protected:
    CommandType m_Type;
    Client m_InvokerClient;
};
