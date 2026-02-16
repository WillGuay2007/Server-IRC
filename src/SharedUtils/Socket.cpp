#include <winsock2.h>
#include <iostream>
#include "Socket.h"

Socket::Socket(int address) {
currentSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(address); // port IRC
    currentAddress = addr;
}

Socket::~Socket() {
    closesocket(currentSocket);
}


void Socket::Send(const char* buffer, int buffersize) {
    if (send(currentSocket, buffer, buffersize, 0) == SOCKET_ERROR) {
        std::cout << "SOCKET SEND ERROR\n";
    };
}

bool Socket::WaitForResponse(char* buffer, int buffersize) {
    int received = recv(currentSocket, buffer, buffersize - 1, 0);
    if (received <= 0) {
        strcpy(buffer, "COULD NOT GET MESSAGE.");
        return false;
    };
    buffer[received] = 0;
    return true;
}

Socket::Socket(void* windowSocket) {
    currentSocket = *(SOCKET*)windowSocket;
    //Pas besoin de mettre l'adresse ici puisque le socket la contient deja.
}