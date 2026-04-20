#include "UserHandler.h"
#include "ServerClient.h"

void UserHandler::Handle(const std::vector<std::string>& params) {
    if (params.empty() || params.size() < 4) {
        client.Send(GeneratePrefix(ERR_NEEDMOREPARAMS) + client.GetNick() + " USER " + ":Not enough parameters\n");
        return;
    }
    if (client.GetRealName() != "*" || client.GetUsername() != "*") {
        client.Send(GeneratePrefix(ERR_ALREADYREGISTERED) + client.GetNick() + " USER " + ":You may not re-register.\n");
        return;
    }
    std::string username = params[0];
    std::string realName = params[3];
    if (realName.empty() || username.empty()) {
        client.Send(GeneratePrefix(ERR_NEEDMOREPARAMS) + client.GetNick() + " USER " + ":Not enough parameters\n");
        return;
    }
    client.SetUsername(username);
    client.SetRealName(realName);
    if (client.CheckIfIsRegistered()) {
        client.Send(GeneratePrefix(RPL_WELCOME) + client.GetNick() + " :Welcome to " + SERVER_NAME + "!\n");
        return;
    } else {
        client.Send("Succesfully set your USER. Please set your NICK now.\n");
    }
}