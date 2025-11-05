#include "Commands/ExitCommand.h"
#include "Constants.h"
#include "SocketIO.h"
#include "Event/EventBus.h"
#include "Event/Events.h"

ExitCommand::ExitCommand(CommandType type, const Client& client) 
    : Command::Command(type, client) {}

void ExitCommand::Execute() const
{
    SocketIO::SendMessage(GetInvoker(), SERVER_LEAVE_MESSAGE);
    SocketIO::Close(GetInvoker());
}