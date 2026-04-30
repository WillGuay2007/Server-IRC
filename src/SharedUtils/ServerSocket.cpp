#include "ServerSocket.h"
#include <winsock2.h>
#include <iostream>

ServerSocket::ServerSocket(int address) : Socket(address) {}
ServerSocket::ServerSocket(void* windowSocket) : Socket(windowSocket) {}

void ServerSocket::StartListening() {
    bind(*(SOCKET*)GetWindowSocket(), (sockaddr*)GetPort(), sizeof(*(sockaddr_in*)GetPort()));
    listen(*(SOCKET*)GetWindowSocket(), 5);
}

ClientSocket* ServerSocket::WaitForConnection() {
    SOCKET* clientWindowSocket = new SOCKET(accept(*(SOCKET*)GetWindowSocket(), nullptr, nullptr));
    if (*clientWindowSocket == INVALID_SOCKET) {
        delete clientWindowSocket;
        std::cout << "accept failed\n";
        return nullptr;
    }
    return new ClientSocket(clientWindowSocket);
}
