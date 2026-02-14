#include "entrypoint.h"
#include <iostream>
#include <string>
#include "ClientServerData.h"

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>

void InitializeWinSock() {
    WSADATA data;
    WSAStartup(MAKEWORD(2,2), &data);
}
void HandleClient(ClientSocket& client) {
    std::cout << "Client connected\n";

    char buffer[500];

    while (true)
    {
        client.WaitForResponse(buffer, sizeof(buffer));
        std::cout << ">> " << buffer;
    }

    std::cout << "Client disconnected\n";
}

void server_start()
{
    InitializeWinSock();
    PrintHello(); //Fonction test

    ServerSocket serverSocket(6667);
    serverSocket.StartListening();

    std::cout << "Server running on port 6667...\n";

    while (true)
    {
        ClientSocket client = serverSocket.WaitForConnection();
        HandleClient(client);
    }

    WSACleanup();
}
