#include "UserHandler.h"
#include "ServerClient.h"

void UserHandler::Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) {
    if (params.empty() || params.size() < 4) {
        clientToHandle.Send(GeneratePrefix(ERR_NEEDMOREPARAMS) + clientToHandle.GetNick() + " USER " + ":Not enough parameters\n");
        return;
    }
    if (clientToHandle.GetRealName() != "*" || clientToHandle.GetUsername() != "*") {
        clientToHandle.Send(GeneratePrefix(ERR_ALREADYREGISTERED) + clientToHandle.GetNick() + " USER " + ":You may not re-register.\n");
        return;
    }
    std::string username = params[0];
    std::string realName = params[3];
    if (realName.empty() || username.empty()) {
        clientToHandle.Send(GeneratePrefix(ERR_NEEDMOREPARAMS) + clientToHandle.GetNick() + " USER " + ":Not enough parameters\n");
        return;
    }
    clientToHandle.SetUsername(username);
    clientToHandle.SetRealName(realName);
    if (clientToHandle.CheckIfIsRegistered()) {
        clientToHandle.Register();
        return;
    } else {
        clientToHandle.Send("Succesfully set your USER. Please set your NICK now.\n");
    }
}