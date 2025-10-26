#pragma once

#include <string>
#include <memory>
#include "Enums.h"
#include <functional>

class Command 
{
public:
    static std::unique_ptr<Command> Create(const std::string& name, const std::vector<std::string>& args, int fd);
    static CommandType GetCommandTypeFromName(const std::string& name);
    virtual ~Command() = default;
    virtual void Execute() const;
    const CommandType GetType() const { return m_Type; }
    const uint32_t GetInvokerDescriptor() const { return m_InvokerFd; }

protected:
    Command() = default;
    Command(CommandType type, int fd);

protected:
    CommandType m_Type;
    int m_InvokerFd;
};