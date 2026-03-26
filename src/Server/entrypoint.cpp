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
#include "handlers.h"
#include "Channel.h"
#include "UnitTest.h"
#include <unordered_map>
#include <functional>

std::vector<Channel*> channels {
    new Channel("#General")
};

std::vector<ServerClient*> clients;

std::unordered_map<std::string, std::function<void(ServerClient&, std::vector<std::string>&)>> commandsMap {
    {"NICK", [](ServerClient& client, std::vector<std::string>& params) {HandleNick(client, params, clients);}},
    {"USER", [](ServerClient& client, std::vector<std::string>& params) {HandleUser(client, params);}},
    {"MOTD", [](ServerClient& client, std::vector<std::string>& params) {HandleMOTD(client, params);}},
    {"PING", [](ServerClient& client, std::vector<std::string>& params) {HandlePing(client, params);}},
    {"JOIN", [](ServerClient& client, std::vector<std::string>& params) {HandleJoin(client, params, channels);}},
};

void GetParameters(std::vector<std::string>& parametersVector, std::string line)
{
    size_t firstSpace = line.find(' ');
    if (firstSpace == std::string::npos) return;

    std::string params = line.substr(firstSpace + 1);
    size_t pos = 0;
    
    while (!params.empty() && params[0] != ':' && (pos = params.find(' ') != std::string::npos))
    {
        if (pos + 1 < params.size() && (params[pos + 1] == ':')) {
            pos++;
            break;
        }
        parametersVector.push_back(params.substr(0, pos));
        params.erase(0, pos + 1);
    }

    if (!params.empty() && params[pos] == ':') {
        parametersVector.push_back(params.substr(1));
        return; //Parce que le : est toujours a la fin.
    }

    if (!params.empty()) parametersVector.push_back(params);
}

void ExecuteCommand(char* receivedLine, ServerClient& client)
{
    std::string line(receivedLine);

    if (line.size() >= 2 && line.substr(line.size() - 2) == "\r\n") line.erase(line.size() - 2);

    std::string command = line.substr(0, line.find(' '));
    std::vector<std::string> parameters;
    GetParameters(parameters, line);

    if (commandsMap.count(command)) {
        commandsMap[command](client, parameters);
    } else {
        std::string msg = "Command " + command + " not found.\n";
        client.GetSocket()->Send(msg.c_str(), msg.size());
    }
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

void RemoveClient(ServerClient* client)
{
    for (auto it = clients.begin(); it != clients.end(); it++)
    {
        if (*it == client)
        {
            clients.erase(it);
            break;
        }
    }
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
            RemoveClient(client);
            delete client;
        });
        clientThread.detach();
    }

    DeInitWinsock2();
}
