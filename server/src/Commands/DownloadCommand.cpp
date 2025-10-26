#include "Commands/DownloadCommand.h"
#include "SocketIO.h"
#include <filesystem>
#include "fstream"

DownloadCommand::DownloadCommand(CommandType type, const std::vector<std::string>& args, int fd)
    : Command::Command(type, fd)
    , m_Args(args)
{
}
void DownloadCommand::Execute() const
{
    std::string fileToDownload = m_Args[0];
    if (fileToDownload.size() <= 0) 
    {
        SocketIO::SendMessage(m_InvokerFd, "No file found of such name");
        return;
    }

    for (const auto& entry : std::filesystem::directory_iterator(".")) 
    {
        if (entry.path().filename().string() == fileToDownload)
        {
            std::printf("[LOG]: Found file %s\n", fileToDownload.c_str());

            std::ifstream fileStream(fileToDownload);
            std::string contents {};
            if (!fileStream.is_open())
            {
                SocketIO::SendMessage(m_InvokerFd, "Could not open specified file");
                return;
            }
            std::string line {};
            while (std::getline(fileStream, line))
            {
                contents.append(line + '\n');
            }
            fileStream.close();

            SocketIO::SendMessage(m_InvokerFd, contents);
        }
    }
}