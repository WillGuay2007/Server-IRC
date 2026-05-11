#pragma once
#include <string>
#include <vector>
#include "ClientSocket.h"
#include <mutex>
#include <sstream>

class ClientIRC
{
private:
    ClientSocket* client;
    std::vector<std::string> messages;
    std::mutex messagesMutex;
    std::string nickname;

    std::string ExtractNickname(const std::string& ircMessage);
    std::string ExtractMessage(const std::string& ircMessage);
    std::string currentChannel = "#chat";
    

public:
    ClientIRC(ClientSocket* clientSocket);

    void HandleIRCMessage(const std::string& ircMessage);
    void SendUserInput(const std::string& input);

    std::vector<std::string> GetMessages();
    std::string GetNickname() const;
};