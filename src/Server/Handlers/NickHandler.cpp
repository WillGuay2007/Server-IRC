#include "NickHandler.h"
#include "ServerClient.h"

std::string NickHandler::Handle(const std::vector<std::string>& params) {
if (params.empty()) {
        return GeneratePrefix(ERR_NONICKNAMEGIVEN) + ":No nickname given\n";
    }
    std::string chosenNick = params[0];
    if (m_clients.FindClientByNick(chosenNick)) {
        return GeneratePrefix(ERR_NICKNAMEINUSE) + m_client.GetNick() + " " + chosenNick + " :Nickname is already in use.\n";
    }
    
    m_client.SetNick(chosenNick);
    if (m_client.CheckIfIsRegistered()) {
        return GeneratePrefix(RPL_WELCOME) + m_client.GetNick() + " :Welcome to " + SERVER_NAME + "!\n";
    } else {
        return "Succesfully set your NICK. Please set your USER now.\n";
    }
}