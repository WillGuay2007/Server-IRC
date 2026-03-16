#include "entrypoint.h"
#include <iostream>
#include <string.h>
#include "ServerSocket.h"
#include "ClientSocket.h"
#include "Winsock2Init.h"
#include <thread>
#include <unordered_map>
#include <functional>
#include "ICommand.h"
#include "ServerCommands.h"


const char* commandStrings[] = {
    "FIND",
    "NICK",
    "JOIN",
};

std::unordered_map<std::string, std::function<ICommand*()>> commandFactory = {
    {"JOIN", []() { return new cmd_join(); }},
    {"FIND", []() { return new cmd_find(); }},
    {"NICK", []() { return new cmd_nick(); }}
};

ICommand* FindCommand(char* receivedLine)
{
    std::string line(receivedLine);

    if (line.size() >= 2 && line.substr(line.size() - 2) == "\r\n") line.erase(line.size() - 2);

    std::string command = line.substr(0, line.find(' '));

    auto it = commandFactory.find(command);

    if (it != commandFactory.end()) return it->second();

    return nullptr;
}

void HandleClient(ClientSocket& client) {
    std::cout << "Client connected\n";

    char buffer[500];

    while (true)
    {
        if (!client.WaitForResponse(buffer, sizeof(buffer)))
        {
            break; //Déconnecter le client si ca fail.
        }
        std::cout << "Client says: " << buffer;
        ICommand* command = FindCommand(buffer);
        command->execute();
        const char* reply = "Hello client\r\n";
        client.Send(reply, (int)strlen(reply));
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
        ClientSocket* client = serverSocket.WaitForConnection();
        if (!client) continue;
        std::thread clientThread([client]() {
            HandleClient(*client);
            delete client;
        });
        clientThread.detach();
    }

    DeInitWinsock2();
}
