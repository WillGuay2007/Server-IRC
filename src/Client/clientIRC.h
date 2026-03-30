#pragma once
#include <string>
#include <vector>

class ClientSocket;

void HandleIRCMessage(
    const std::string& ircMessage,
    ClientSocket* client,
    std::vector<std::string>& messages,
    std::string& nickname
);