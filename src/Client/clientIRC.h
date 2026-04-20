#pragma once
#include <string>
#include <vector>
#include "ClientSocket.h"
#include <mutex>

class ClientIRC
{
private:
    ClientSocket* client;
    std::vector<std::string> messages;
    std::mutex messagesMutex;
    std::string nickname;

    std::string ExtractNickname(const std::string& ircMessage);
    std::string ExtractMessage(const std::string& ircMessage);

public:
    ClientIRC(ClientSocket* clientSocket);

    void HandleIRCMessage(const std::string& ircMessage);

    std::vector<std::string> GetMessages();
    std::string GetNickname() const;
};