#include "handlers.h"

//Exemple: :ScaryServer 431 
std::string GeneratePrefix(ServerClient& client, EServerResponse response) {
    return ":" + serverName + " " + std::to_string((int)response) + " ";
}

void SendStringResponse(ServerClient& client, std::string response) {
    client.GetSocket()->Send(response.c_str(), response.size()); 
}

void HandleJoin(ServerClient& client, std::vector<std::string>& parameters, std::vector<Channel*> channels) {
    if (parameters.empty()) {

        return;
    }
    std::string channelName = parameters[0];
    if (client.IsInChannel(channelName)) {
        SendStringResponse(client, "You are already a member of channel: " + channelName +  "\n");
    }
    for (int i = 0; i < channels.size(); i++) {
        if (channels[i]->GetName() == channelName) {
            channels[i]->AddMember(&client);
            client.AddChannel(channels[i]);
            SendStringResponse(client, "Executing command: JOIN\nJoining " + channelName + " channel\n");
            return;
        } else {
            std::cout << parameters[0] << " " << channels[i]->GetName();
            SendStringResponse(client, "Channel " + parameters[0] + " is invalid.\n");
        }
    }
}

void HandleMOTD(ServerClient& client, std::vector<std::string>& parameters) {
    if (parameters.empty()) {

        return;
    }
}

void HandleNick(ServerClient& client, std::vector<std::string>& parameters, std::vector<ServerClient*> clients){
    if (parameters.empty()) {
        SendStringResponse(client, 
                GeneratePrefix(client, ERR_NONICKNAMEGIVEN) +
                ":No nickname given\n"
            );
        return;
    }
    std::string chosenNick = parameters[0];
    for (ServerClient* _client : clients) {
        if (_client->GetNick() == chosenNick) {
            if (client.GetSocket() == _client->GetSocket()) continue;
            SendStringResponse(client, 
                GeneratePrefix(client, ERR_NICKNAMEINUSE)
                + client.GetNick() + " " + chosenNick + " :Nickname is already in use.\n"
            );
            return;
        }
    }
    
    client.SetNick(chosenNick);
    SendStringResponse(client, "Set nick to: " + client.GetNick());
}

void HandleUser(ServerClient& client, std::vector<std::string>& parameters) {
    if (parameters.empty() || parameters.size() < 4) {
        SendStringResponse(client,
             GeneratePrefix(client, ERR_NEEDMOREPARAMS) + client.GetNick() + " USER " + ":Not enough parameters\n"
            );
        return;
    }
    if (client.GetRealName().length() != 0 || client.GetUsername().length() != 0) {
        SendStringResponse(client,
             GeneratePrefix(client, ERR_ALREADYREGISTERED) + client.GetNick() + " USER " + ":You may not re-register.\n"
            );
        return;
    }
    std::string username = parameters[0];
    std::string realName = parameters[3]; //TODO: Implementer le colon dans GetParameters pour message avec espace
    if (realName.length() <= 0 || username.length() <= 0) {
        SendStringResponse(client,
             GeneratePrefix(client, ERR_NEEDMOREPARAMS) + client.GetNick() + " USER " + ":Not enough parameters\n"
            );
        return;
    }
    client.SetUsername(username);
    client.SetRealName(realName);
    SendStringResponse(client, "Succesfully executed command USER.\n");
}

void HandlePing(ServerClient& client, std::vector<std::string>& parameters) {
    if (parameters.empty()) {
        SendStringResponse(client, "PONG\n");
        return;
    }
    SendStringResponse(client, "PONG " + parameters[0] + "\n");
}
