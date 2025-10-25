#pragma once

#include <string>
#include <memory>
#include "Enums.h"
#include <functional>

class Command 
{
public:
    static std::unique_ptr<Command> Create(const std::string& name, int fd);
    static CommandType GetCommandTypeFromName(const std::string& name);
    virtual ~Command() = default;
    virtual void Execute(std::function<void(int fd, const std::string& response)> onExecuteComplete) const;
    const CommandType GetType() const { return m_Type; }
    const uint32_t GetInvokerDescriptor() const { return m_InvokerFd; }

protected:
    Command() = default;
    Command(CommandType type, int fd);

protected:
    CommandType m_Type;
    uint32_t m_InvokerFd;
};