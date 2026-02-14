#include "ClientServerData.h"

ClientSocket::ClientSocket(int address) : Socket(address) {}
ClientSocket::ClientSocket(SOCKET windowSocket) : Socket(windowSocket) {}

void ClientSocket::Connect() {
    connect(currentSocket, (sockaddr*)&currentAddress, sizeof(currentAddress));
}
