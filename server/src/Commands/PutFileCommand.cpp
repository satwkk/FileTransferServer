#include "Commands/PutFileCommand.h"
#include <fstream>
#include <unistd.h>
#include "SocketIO.h"

PutFileCommand::PutFileCommand(CommandType type, const std::vector<std::string>& args, int fd)
    : Command::Command(type, fd)
    , m_Args(args)
{
}

void PutFileCommand::Execute() const
{
    if (m_Args.size() < 2) 
    {
        SocketIO::SendMessage(m_InvokerFd, "Invalid put command");
        return;
    }

    const std::string& fileName = m_Args[0];

    if (fileName.starts_with("..")) 
    {
        SocketIO::SendMessage(m_InvokerFd, "Sus command detected");
        return;
    }

    const std::vector<char> stream (m_Args[1].begin(), m_Args[1].end());

    std::ofstream fStream(fileName);
    if (fStream.is_open()) 
    {
        fStream.write(stream.data(), stream.size());
    }

    SocketIO::SendMessage(m_InvokerFd, "Successfully wrote to disc\n");
    fStream.close();
}
