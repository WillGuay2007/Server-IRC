#include "NickHandler.h"
#include "ServerClient.h"

void NickHandler::Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) {
    if (params.empty()) {
        clientToHandle.Send(GeneratePrefix(ERR_NONICKNAMEGIVEN) + ":No nickname given\n");
        return;
    }
    std::string chosenNick = params[0];
    if (m_clients.FindClientByNick(chosenNick)) {
        clientToHandle.Send(GeneratePrefix(ERR_NICKNAMEINUSE) + clientToHandle.GetNick() + " " + chosenNick + " :Nickname is already in use.\n");
        return;
    }
    
    clientToHandle.SetNick(chosenNick);
    clientToHandle.Send("Succesfully set your nick to: " + clientToHandle.GetNick() + "\n");
    if (clientToHandle.CanRegister()) {
        clientToHandle.Register();
        return;
    }
}