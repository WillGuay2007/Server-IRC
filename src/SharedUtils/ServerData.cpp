#include "ClientServerData.h"

ServerSocket::ServerSocket(int address) : Socket(address) {}

void ServerSocket::Connect() {
    bind(currentSocket, (sockaddr*)&currentAddress, sizeof(currentAddress));
    listen(currentSocket, 5);
}

SOCKET ServerSocket::WaitForConnection() {
    return accept(currentSocket, nullptr, nullptr);
}
