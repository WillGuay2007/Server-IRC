#include "handlers.h"

//Exemple: :ScaryServer 431 
std::string GeneratePrefix(ServerClient& client, EServerResponse response) {
    return ":" + serverName + " " + std::to_string((int)response) + " ";
}

void SendStringResponse(ServerClient& client, std::string response) {
    client.GetSocket()->Send(response.c_str(), response.size()); 
}

void HandleJoin(ServerClient& client, std::vector<std::string>& parameters, std::vector<Channel*>& channels) {
    if (parameters.empty()) {

        return;
    }
    std::string channelName = parameters[0];
    if (client.IsInChannel(channelName)) {
        SendStringResponse(client, "You are already a member of channel: " + channelName +  "\n");
        return;
    }
    for (int i = 0; i < channels.size(); i++) {
        if (channels[i]->GetName() == channelName) {
            channels[i]->AddMember(&client);
            client.AddChannel(channels[i]);
            SendStringResponse(client, "Executing command: JOIN\nJoining " + channelName + " channel\n");
            return;
        }
    }
    SendStringResponse(client, "Channel " + parameters[0] + " is invalid.\n");
}

void HandleMOTD(ServerClient& client, std::vector<std::string>& parameters) {
    if (!MOTD.empty()) {
        SendStringResponse(client, MOTD);
    } else {
        SendStringResponse(client, GeneratePrefix(client, ERR_NOMOTD) + ":The server does not have a message of the day.\n");
    }
}

void HandleNick(ServerClient& client, std::vector<std::string>& parameters, std::vector<ServerClient*>& clients){
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
            if (client.GetSocket() == _client->GetSocket()) {
                SendStringResponse(client, "You already have this username\n");
                return;
            }
            SendStringResponse(client, 
                GeneratePrefix(client, ERR_NICKNAMEINUSE)
                + client.GetNick() + " " + chosenNick + " :Nickname is already in use.\n"
            );
            return;
        }
    }
    
    client.SetNick(chosenNick);
    if (CheckIfUserIsRegistered(client)) {
        SendStringResponse(client, GeneratePrefix(client, RPL_WELCOME) + client.GetNick() + ":Welcome to " + serverName + "!\n");
    } else {
        SendStringResponse(client, "Succesfully set your NICK. Please set your USER now.\n");
    }
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
    std::string realName = parameters[3];
    if (realName.length() <= 0 || username.length() <= 0) {
        SendStringResponse(client,
             GeneratePrefix(client, ERR_NEEDMOREPARAMS) + client.GetNick() + " USER " + ":Not enough parameters\n"
            );
        return;
    }
    client.SetUsername(username);
    client.SetRealName(realName);
    if (CheckIfUserIsRegistered(client)) {
        SendStringResponse(client, GeneratePrefix(client, RPL_WELCOME) + client.GetNick() + " :Welcome to " + serverName + "!\n");
    } else {
         SendStringResponse(client, "Succesfully set your USER. Please set your NICK now.\n");
    }
}

void HandlePing(ServerClient& client, std::vector<std::string>& parameters) {
    if (parameters.empty()) {
        SendStringResponse(client, "PONG\n");
        return;
    }
    SendStringResponse(client, "PONG " + parameters[0] + "\n");
}

bool CheckIfUserIsRegistered(ServerClient& client) {
    return (client.GetNick() != "*" && client.GetUsername() != "" && client.GetRealName() != "");
}