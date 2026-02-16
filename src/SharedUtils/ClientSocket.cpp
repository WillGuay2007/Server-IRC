#include "ClientSocket.h"
#include <winsock2.h>
#include <iostream>

ClientSocket::ClientSocket(int address) : Socket(address) {}
ClientSocket::ClientSocket(void* windowSocket) : Socket(*(SOCKET*)windowSocket) {}

void ClientSocket::Connect() {
    currentAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(currentSocket, (sockaddr*)&currentAddress, sizeof(currentAddress)) == SOCKET_ERROR)
    {
        std::cout << "Connection failed\n";
    }
}

