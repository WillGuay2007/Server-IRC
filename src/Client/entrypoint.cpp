#include "entrypoint.h"
#include <iostream>
#include <string>
#include "ClientServerData.h"

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

void client_start()
{
    WSADATA data;
    WSAStartup(MAKEWORD(2,2), &data);

    // 1. Créer le client socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6667); // IRC port
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 2. Se connecter au port. Si il y'a une erreur, return.
    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        std::cerr << "Connection failed\n";
        return;
    }

    std::cout << "Connected to server\n";

    // 3. Envoyer un message au serveur.
    std::string msg = "Hello server\r\n";
    send(sock, msg.c_str(), (int)msg.size(), 0);

    // 4. Attendre la réponse du serveur
    char buffer[512];
    int received = recv(sock, buffer, sizeof(buffer) - 1, 0);

    if (received > 0)
    {
        buffer[received] = 0;
        std::cout << "Server says: " << buffer;
    }

    // 5. Terminer la connection après avoir recu le message.
    closesocket(sock);
    WSACleanup();
}
