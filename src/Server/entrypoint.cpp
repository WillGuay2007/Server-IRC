#include "entrypoint.h"
#include <iostream>
#include <string>
#include "ClientServerData.h"

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>

void server_start()
{
    // 1️. Initializer les fonctionnalités.
    WSADATA data;
    WSAStartup(MAKEWORD(2,2), &data);

    PrintHello();

    // 1. Créer le server socket.
    ServerSocket serverSocket(6667);

    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Écouter son port.
    serverSocket.Connect();

    std::cout << "Server running on port 6667...\n";

    // 3. Attendre un client
    while (true)
    {
        SOCKET client = serverSocket.WaitForConnection();
        std::cout << "Client connected\n";

        char buffer[512];

        // 4. Attendre les données du client
        while (true)
        {
            serverSocket.WaitForResponse(client, buffer, sizeof(buffer));
            std::cout << ">> " << buffer;
        }

        std::cout << "Client disconnected\n";
        closesocket(client);
    }

    serverSocket.Close();
    WSACleanup();
}
