#include "entrypoint.h"
#include <iostream>
#include <string.h>
#include "ServerSocket.h"
#include "ClientSocket.h"
#include "Winsock2Init.h"
#include "Channel.h"
#include <thread>
#include "ServerClient.h"
#include <vector>
#include "ServerResponses.h"

std::vector<std::string> commands {
    "NICK",
    "USER",
    "JOIN",
    "PRIVMSG",
    "PING"
};

std::vector<ServerClient*> clients;

const std::string MOTD = "You either cum in the sink or sink in the cum\n";
const std::string serverName = "ScaryServer";

std::vector<Channel*> channels {
    new Channel("#General")
};

//Exemple: :ScaryServer 431 
std::string GeneratePrefix(ServerClient& client, EServerResponse response) {
    return ":" + serverName + " " + std::to_string((int)response) + " ";
}

void SendStringResponse(ServerClient& client, std::string response) {
    client.GetSocket()->Send(response.c_str(), response.size()); 
}

//First parameter: Channel
void HandleJoin(ServerClient& client, std::vector<std::string>& parameters) {
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

void HandleNick(ServerClient& client, std::vector<std::string>& parameters) {
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

void GetParameters(std::vector<std::string>& parametersVector, std::string line)
{
    size_t firstSpace = line.find(' ');

    if (firstSpace == std::string::npos) return;

    std::string params = line.substr(firstSpace + 1);

    size_t pos = 0;

    while ((pos = params.find(' ')) != std::string::npos)
    {
        parametersVector.push_back(params.substr(0, pos));
        params.erase(0, pos + 1);
    }

    if (!params.empty()) parametersVector.push_back(params);
}

void ExecuteCommand(char* receivedLine, ServerClient& client)
{
    std::string line(receivedLine);

    if (line.size() >= 2 && line.substr(line.size() - 2) == "\r\n") line.erase(line.size() - 2);

    std::string command = line.substr(0, line.find(' '));

    for (int i = 0; i < commands.size(); i++) {
        if (command == commands[i]) {
            std::vector<std::string> parameters = std::vector<std::string>();
            GetParameters(parameters, line);
            if (command == "JOIN") {
                HandleJoin(client, parameters);
            } else if (command == "MOTD") {
                HandleMOTD(client, parameters);
            } else if (command == "NICK") {
                HandleNick(client, parameters);
            } else if (command == "USER") {
                HandleUser(client, parameters);
            } else if (command == "PING") {
                HandlePing(client, parameters);
            }
            return;
        }
    }
    std::string msg = "Command " + command + " not found.\n";
    client.GetSocket()->Send(msg.c_str(), msg.size());
}

void HandleClient(ServerClient& client) {
    std::cout << "Client connected\n";

    char buffer[500];

    while (true)
    {
        if (!client.GetSocket()->WaitForResponse(buffer, sizeof(buffer)))
        {
            break; //Déconnecter le client si ca fail.
        }

        std::string clientResponse(buffer);

        size_t pos;
        while ((pos = clientResponse.find("\r\n")) != std::string::npos)
        {
            std::string line = clientResponse.substr(0, pos);

            clientResponse.erase(0, pos + 2);

            ExecuteCommand((char*)line.c_str(), client);
        }
    }


    std::cout << "Client disconnected\n";
}

void server_start()
{
    InitWinsock2();

    ServerSocket serverSocket(6667);
    serverSocket.StartListening();

    std::cout << "Server running on port 6667...\n";

    while (true)
    {
        ClientSocket* clientSocket = serverSocket.WaitForConnection();
        ServerClient* client = new ServerClient(clientSocket);
        if (!client) continue;
        clients.push_back(client);
        std::thread clientThread([client]() {
            HandleClient(*client);
            delete client;
        });
        clientThread.detach();
    }

    DeInitWinsock2();
}
