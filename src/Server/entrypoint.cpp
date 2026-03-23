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

std::string commands[256] {
    "NICK",
    "USER",
    "JOIN",
    "PRIVMSG"
};

std::vector<Channel*> channels {
    new Channel("#General")
};

//First parameter: Channel
void HandleJoin(ServerClient& client, std::vector<std::string>& parameters) {
    if (client.IsInChannel(parameters[0])) {
        std::string response("You are already a member of channel: " + parameters[0] +  "\n");
        client.GetSocket()->Send(response.c_str(), response.size());
    }
    for (int i = 0; i < channels.size(); i++) {
        if (channels[i]->GetName() == parameters[0]) {
            std::string response("Executing command: JOIN\nJoining " + parameters[0] + " channel\n");
            channels[i]->AddMember(&client);
            client.AddChannel(channels[i]);
            client.GetSocket()->Send(response.c_str(), response.size());
            return;
        } else {
            std::cout << parameters[0] << " " << channels[i]->GetName();
            std::string response("Channel " + parameters[0] + " is invalid.\n");
            client.GetSocket()->Send(response.c_str(), response.size()); 
        }
    }
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

    for (int i = 0; i < (sizeof(commands) / sizeof(commands[0])); i++) {
        if (command == commands[i]) {
            std::vector<std::string> parameters = std::vector<std::string>();
            GetParameters(parameters, line);
            if (parameters.empty()) return;
            if (command == "JOIN") {
                HandleJoin(client, parameters);
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
        ExecuteCommand(buffer, client);
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
        std::thread clientThread([client]() {
            HandleClient(*client);
            delete client;
        });
        clientThread.detach();
    }

    DeInitWinsock2();
}
