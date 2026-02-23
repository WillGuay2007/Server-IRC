#include "ServerSocket.h"
#include <winsock2.h>
#include <iostream>

ServerSocket::ServerSocket(int address) : Socket(address) {}
ServerSocket::ServerSocket(void* windowSocket) : Socket(windowSocket) {}

void ServerSocket::StartListening() {
    bind(*(SOCKET*)GetWindowSocket(), (sockaddr*)GetAddress(), sizeof(*(sockaddr_in*)GetAddress()));
    listen(*(SOCKET*)GetWindowSocket(), 5);
}

ClientSocket* ServerSocket::WaitForConnection() {
    SOCKET clientWindowSocket = accept(*(SOCKET*)GetWindowSocket(), nullptr, nullptr);
    if (clientWindowSocket == INVALID_SOCKET) {
        std::cout << "accept failed\n";
        return nullptr;
    }
    return new ClientSocket(&clientWindowSocket);
}
