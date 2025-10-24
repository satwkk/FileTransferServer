#pragma once

#include <string>
#include <memory>
#include "Enums.h"

class Command 
{
public:
    static std::unique_ptr<Command> Create(const std::string& name, int fd);
    static CommandType GetCommandTypeFromName(const std::string& name);
    virtual ~Command() = default;
    virtual void Execute() const;
    const CommandType GetType() const { return m_Type; }

private:
protected:
    Command() = default;
    Command(CommandType type, int fd);

protected:
    CommandType m_Type;
    uint32_t m_InvokerFd;
};