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
    const std::vector<char> stream (m_Args[1].begin(), m_Args[1].end());

    std::printf("Filename: %s\n", fileName.c_str());
    std::printf("Received bytes: %s\n", stream.data());

    std::ofstream fStream(fileName);
    if (fStream.is_open()) 
    {
        fStream.write(stream.data(), stream.size());
    }

    SocketIO::SendMessage(m_InvokerFd, "Successfully wrote to disc\n");
    fStream.close();
}
