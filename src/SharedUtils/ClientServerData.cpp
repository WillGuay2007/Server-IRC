#include "ClientServerData.h"

Socket::Socket(int address) {
currentSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(address); // port IRC
    currentAddress = addr;
}

Socket::~Socket() {}
void Socket::Close() {
    closesocket(currentSocket);
}

void Socket::Send(SOCKET targetedSocket, char* buffer, int buffersize) {
    if (send(targetedSocket, buffer, buffersize, 0) == SOCKET_ERROR) {
        std::cout << "SOCKET SEND ERROR\n";
    };
}

void Socket::WaitForResponse(SOCKET desiredSocket, char* buffer, int bufferSize) {
    int received = recv(desiredSocket, buffer, bufferSize - 1, 0);
    if (received <= 0) {
        buffer = (char*)"COULD NOT GET CLIENT MESSAGE.";
        return;
    };
    buffer[received] = 0;
}

void PrintHello() {
   std::cout << "Hello!\n";
}