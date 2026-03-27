#include "handlers.h"

//Exemple: :ScaryServer 431 
std::string GeneratePrefix(ServerClient& client, EServerResponse response) {
    return ":" + serverName + " " + std::to_string((int)response) + " ";
}

void SendStringResponse(ServerClient& client, std::string response) {
    client.GetSocket()->Send(response.c_str(), response.size()); 
}

std::string HandleJoin(ServerClient& client, std::vector<std::string>& parameters, std::vector<Channel*>& channels) {
    if (parameters.empty()) {
        return GeneratePrefix(client, ERR_NEEDMOREPARAMS) + client.GetNick() + " JOIN " + ":Not enough parameters\n";
    }
    std::string channelName = parameters[0];
    if (client.IsInChannel(channelName)) {
        return "You are already a member of channel: " + channelName +  "\n";
    }
    for (int i = 0; i < channels.size(); i++) {
        if (channels[i]->GetName() == channelName) {
            channels[i]->AddMember(&client);
            client.AddChannel(channels[i]);
            return "Executing command: JOIN\nJoining " + channelName + " channel\n";
        }
    }
    return "Channel " + parameters[0] + " is invalid.\n";
}

std::string HandleMOTD(ServerClient& client, std::vector<std::string>& parameters) {
    if (!MOTD.empty()) {
        return MOTD;
    } else {
        return GeneratePrefix(client, ERR_NOMOTD) + ":The server does not have a message of the day.\n";
    }
}

std::string HandleNick(ServerClient& client, std::vector<std::string>& parameters, std::vector<ServerClient*>& clients){
    if (parameters.empty()) {
        return GeneratePrefix(client, ERR_NONICKNAMEGIVEN) + ":No nickname given\n";
    }
    std::string chosenNick = parameters[0];
    for (ServerClient* _client : clients) {
        if (_client->GetNick() == chosenNick) {
            //TODO: Change socket comparaison because handlers should not need to use sockets and it will break unit tests...
            if (client.GetSocket() == _client->GetSocket()) {
                return "You already have this username\n";
            }
            return GeneratePrefix(client, ERR_NICKNAMEINUSE) + client.GetNick() + " " + chosenNick + " :Nickname is already in use.\n";
        }
    }
    
    client.SetNick(chosenNick);
    if (CheckIfUserIsRegistered(client)) {
        return GeneratePrefix(client, RPL_WELCOME) + client.GetNick() + ":Welcome to " + serverName + "!\n";
    } else {
        return "Succesfully set your NICK. Please set your USER now.\n";
    }
}

std::string HandleUser(ServerClient& client, std::vector<std::string>& parameters) {
    if (parameters.empty() || parameters.size() < 4) {
        return GeneratePrefix(client, ERR_NEEDMOREPARAMS) + client.GetNick() + " USER " + ":Not enough parameters\n";
    }
    if (client.GetRealName().length() != 0 || client.GetUsername().length() != 0) {
        return GeneratePrefix(client, ERR_ALREADYREGISTERED) + client.GetNick() + " USER " + ":You may not re-register.\n";
    }
    std::string username = parameters[0];
    std::string realName = parameters[3];
    if (realName.length() <= 0 || username.length() <= 0) {
        return GeneratePrefix(client, ERR_NEEDMOREPARAMS) + client.GetNick() + " USER " + ":Not enough parameters\n";
    }
    client.SetUsername(username);
    client.SetRealName(realName);
    if (CheckIfUserIsRegistered(client)) {
        return GeneratePrefix(client, RPL_WELCOME) + client.GetNick() + " :Welcome to " + serverName + "!\n";
    } else {
         return "Succesfully set your USER. Please set your NICK now.\n";
    }
}

std::string HandlePing(ServerClient& client, std::vector<std::string>& parameters) {
    if (parameters.empty()) {
        return "PONG\n";
    }
    return "PONG " + parameters[0] + "\n";
}

bool CheckIfUserIsRegistered(ServerClient& client) {
    return (client.GetNick() != "*" && client.GetUsername() != "" && client.GetRealName() != "");
}