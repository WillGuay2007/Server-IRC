#include "entrypoint.h"
#include <iostream>
#include <string>
#include "ServerSocket.h"
#include "ClientSocket.h"

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
        if (!client.WaitForResponse(buffer, sizeof(buffer))) continue;
        std::cout << ">> " << buffer;
    }

    std::cout << "Client disconnected\n";
}

void server_start()
{
    InitializeWinSock();

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
