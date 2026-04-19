#include "UserHandler.h"
#include "ServerClient.h"

std::string UserHandler::Handle(const std::vector<std::string>& params) {
    if (params.empty() || params.size() < 4) {
        return GeneratePrefix(ERR_NEEDMOREPARAMS) + m_client.GetNick() + " USER " + ":Not enough parameters\n";
    }
    if (m_client.GetRealName() != "*" || m_client.GetUsername() != "*") {
        return GeneratePrefix(ERR_ALREADYREGISTERED) + m_client.GetNick() + " USER " + ":You may not re-register.\n";
    }
    std::string username = params[0];
    std::string realName = params[3];
    if (realName.empty() || username.empty()) {
        return GeneratePrefix(ERR_NEEDMOREPARAMS) + m_client.GetNick() + " USER " + ":Not enough parameters\n";
    }
    m_client.SetUsername(username);
    m_client.SetRealName(realName);
    if (m_client.CheckIfIsRegistered()) {
        return GeneratePrefix(RPL_WELCOME) + m_client.GetNick() + " :Welcome to " + SERVER_NAME + "!\n";
    } else {
         return "Succesfully set your USER. Please set your NICK now.\n";
    }
}