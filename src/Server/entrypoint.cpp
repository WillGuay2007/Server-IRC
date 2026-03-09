#include "entrypoint.h"
#include <iostream>
#include <string.h>
#include "ServerSocket.h"
#include "ClientSocket.h"
#include "Winsock2Init.h"
#include <thread>
#include "ICommand.h"
#include "ServerCommands.h"


const char* commandStrings[] = {
    "FIND",
    "NICK",
    "JOIN",
};

void FindAndExecuteCommand(char* receivedLine) {
    std::string line(receivedLine);
    if (line.size() >= 2 && line.substr(line.size() - 2) == "\r\n") {
        line.erase(line.size() - 2);
    }
    int sizeOfCmdArray = sizeof(commandStrings) / sizeof(commandStrings[0]);
    for (int i = 0; i < sizeOfCmdArray; i++) {
        std::string possibleCommandString(commandStrings[i]);
        if (line == possibleCommandString) {
            std::cout << "Command found: " << possibleCommandString << std::endl;
        }
    }
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
        FindAndExecuteCommand(buffer);
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
