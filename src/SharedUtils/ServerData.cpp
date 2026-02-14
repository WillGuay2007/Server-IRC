#include "ClientServerData.h"

ServerSocket::ServerSocket(int address) : Socket(address) {}
ServerSocket::ServerSocket(SOCKET windowSocket) : Socket(windowSocket) {}

void ServerSocket::StartListening() {
    bind(currentSocket, (sockaddr*)&currentAddress, sizeof(currentAddress));
    listen(currentSocket, 5);
}

ClientSocket ServerSocket::WaitForConnection() {
    SOCKET clientWindowSocket = accept(currentSocket, nullptr, nullptr);
    return ClientSocket(clientWindowSocket);
}
