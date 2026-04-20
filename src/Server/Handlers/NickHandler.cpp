#include "NickHandler.h"
#include "ServerClient.h"

void NickHandler::Handle(const std::vector<std::string>& params) {
    if (params.empty()) {
        client.Send(GeneratePrefix(ERR_NONICKNAMEGIVEN) + ":No nickname given\n");
        return;
    }
    std::string chosenNick = params[0];
    if (m_clients.FindClientByNick(chosenNick)) {
        client.Send(GeneratePrefix(ERR_NICKNAMEINUSE) + client.GetNick() + " " + chosenNick + " :Nickname is already in use.\n");
        return;
    }
    
    client.SetNick(chosenNick);
    if (client.CheckIfIsRegistered()) {
        client.Send(GeneratePrefix(RPL_WELCOME) + client.GetNick() + " :Welcome to " + SERVER_NAME + "!\n");
        return;
    } else {
        client.Send("Succesfully set your NICK. Please set your USER now.\n");
    }
}