#include "NickHandler.h"
#include "ServerClient.h"

std::string NickHandler::Handle(std::vector<std::string>& params) {
if (params.empty()) {
        return GeneratePrefix(ERR_NONICKNAMEGIVEN) + ":No nickname given\n";
    }
    std::string chosenNick = params[0];
    for (ServerClient* _client : m_clients) {
        if (_client->GetNick() == chosenNick) {
            //TODO: Change socket comparaison because handlers should not need to use sockets and it will break unit tests...
            if (m_client.GetSocket() == _client->GetSocket()) {
                return "You already have this username\n";
            }
            return GeneratePrefix(ERR_NICKNAMEINUSE) + m_client.GetNick() + " " + chosenNick + " :Nickname is already in use.\n";
        }
    }
    
    m_client.SetNick(chosenNick);
    if (m_client.CheckIfIsRegistered()) {
        return GeneratePrefix(RPL_WELCOME) + m_client.GetNick() + " :Welcome to " + SERVER_NAME + "!\n";
    } else {
        return "Succesfully set your NICK. Please set your USER now.\n";
    }
}