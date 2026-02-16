#pragma once
#include "ISocket.h"
#include "winsock2.h" //TODO: Enlever cette dependance d'une facon ou d'une autre.

class Socket : public ISocket {
protected:
    SOCKET currentSocket;
    sockaddr_in currentAddress;

public:
    void Send(const char* buffer, int buffersize) override;
    bool WaitForResponse(char* buffer, int buffersize) override;
    Socket(int address);
    Socket(void* windowSocket);
    virtual ~Socket();
};