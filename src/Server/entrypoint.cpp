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
#include <vector>

std::vector<std::string> commands {
    "NICK",
    "USER",
    "JOIN",
    "PRIVMSG",
    "PING"
};

std::vector<Channel*> channels {
    new Channel("#General")
};

std::vector<ServerClient*> clients;

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
                HandleJoin(client, parameters, channels);
            } else if (command == "MOTD") {
                HandleMOTD(client, parameters);
            } else if (command == "NICK") {
                HandleNick(client, parameters, clients);
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
