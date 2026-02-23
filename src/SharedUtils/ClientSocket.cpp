#include "ClientSocket.h"
#include <winsock2.h>
#include <iostream>

ClientSocket::ClientSocket(int address) : Socket(address) {}
ClientSocket::ClientSocket(void* windowSocket) : Socket(windowSocket) {}

void ClientSocket::Connect() {
    (*(sockaddr_in*)GetAddress()).sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(*(SOCKET*)GetWindowSocket(), (sockaddr*)GetAddress(), sizeof(*(sockaddr_in*)GetAddress())) == SOCKET_ERROR)
    {
        std::cout << "Connection failed\n";
    }
}

void ClientSocket::Send(const char* buffer, int buffersize) {
    if (send(*(SOCKET*)GetWindowSocket(), buffer, buffersize, 0) == SOCKET_ERROR) {
        std::cout << "SOCKET SEND ERROR\n";
    };
}

bool ClientSocket::WaitForResponse(char* buffer, int buffersize) {
    int received = recv(*(SOCKET*)GetWindowSocket(), buffer, buffersize - 1, 0);
    if (received == 0) {
        std::cout << "Socket disconnected\n";
        return false;
    } else if (received <= 0) {
        std::cout << "Socket error: " << WSAGetLastError() << "\n";
        return false;
    }
    buffer[received] = 0;
    return true;
}