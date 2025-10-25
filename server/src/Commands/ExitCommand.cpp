#include "Commands/ExitCommand.h"
#include "Constants.h"
#include "SocketIO.h"
#include "Event/EventBus.h"
#include "Event/Events.h"

ExitCommand::ExitCommand(CommandType type, int fd) 
    : Command::Command(type, fd) {}

void ExitCommand::Execute(std::function<void(int fd, const std::string &response)> onExecuteComplete) const
{
    onExecuteComplete(m_InvokerFd, SERVER_LEAVE_MESSAGE);
    SocketIO::Close(m_InvokerFd);
}