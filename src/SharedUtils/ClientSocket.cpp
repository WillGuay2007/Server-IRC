#include "ClientSocket.h"
#include <winsock2.h>
#include <iostream>

ClientSocket::ClientSocket(int address, char* ipAddress) : Socket(address, ipAddress) {}
ClientSocket::ClientSocket(void* windowSocket) : Socket(windowSocket) {}

void ClientSocket::Connect() {
    (*(sockaddr_in*)GetPort()).sin_addr.s_addr = inet_addr(GetIpAddress());

    if (connect(*(SOCKET*)GetWindowSocket(), (sockaddr*)GetPort(), sizeof(*(sockaddr_in*)GetPort())) == SOCKET_ERROR)
    {
        std::cout << "Connection failed\n";
    }
}

void ClientSocket::Send(const char* buffer, int buffersize) {
    int result = send(*(SOCKET*)GetWindowSocket(), buffer, buffersize, 0);

    if (result == SOCKET_ERROR)
    {
        std::cout << "SOCKET SEND ERROR: " << WSAGetLastError() << std::endl;
    }
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

void ClientSocket::Disconnect() {
    closesocket(*(SOCKET*)GetWindowSocket());
}