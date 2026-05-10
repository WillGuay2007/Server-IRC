#include "NickHandler.h"
#include "ServerClient.h"

void NickHandler::Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) {
    if (params.empty()) {
        clientToHandle.Send(GeneratePrefix(ERR_NONICKNAMEGIVEN) + ":No nickname given\r\n");
        return;
    }
    std::string chosenNick = params[0];
    if (m_clients.FindClientByNick(chosenNick)) {
        clientToHandle.Send(GeneratePrefix(ERR_NICKNAMEINUSE) + clientToHandle.GetNick() + " " + chosenNick + " :Nickname is already in use.\r\n");
        return;
    }
    
    clientToHandle.SetNick(chosenNick);
    if (clientToHandle.CanRegister()) {
        clientToHandle.Register();
        return;
    }
}