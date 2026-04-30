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
    if (clientToHandle.CheckIfIsRegistered()) {
        clientToHandle.Send(GeneratePrefix(RPL_WELCOME) + clientToHandle.GetNick() + " :Welcome to " + SERVER_NAME + "!\n");
        return;
    } else {
        clientToHandle.Send("Succesfully set your NICK. Please set your USER now.\n");
    }
}