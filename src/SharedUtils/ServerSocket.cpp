#include "ServerSocket.h"
#include <winsock2.h>


ServerSocket::ServerSocket(int address) : Socket(address) {}
ServerSocket::ServerSocket(void* windowSocket) : Socket(*(SOCKET*)windowSocket) {}

void ServerSocket::StartListening() {
    bind(currentSocket, (sockaddr*)&currentAddress, sizeof(currentAddress));
    listen(currentSocket, 5);
}

ClientSocket ServerSocket::WaitForConnection() {
    SOCKET clientWindowSocket = accept(currentSocket, nullptr, nullptr);
    return ClientSocket(clientWindowSocket);
}
