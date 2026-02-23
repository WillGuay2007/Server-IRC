#include "entrypoint.h"
#include <iostream>
#include <cstring>
#include "ServerSocket.h"
#include "ClientSocket.h"
#include "Winsock2Init.h"
#include <thread>

void HandleClient(ClientSocket& client) {
    std::cout << "Client connected\n";

    char buffer[500];

    while (true)
    {
        if (!client.WaitForResponse(buffer, sizeof(buffer)))
        {
            break;
        }
        std::cout << "Client says: " << buffer;

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
