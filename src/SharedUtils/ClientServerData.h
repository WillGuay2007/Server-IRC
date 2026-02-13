#pragma once
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <iostream>

class ISocket {
public:
    virtual void Connect() = 0;
    virtual void Send(SOCKET targetedSocket, char* buffer, int buffersize) = 0;
    virtual void WaitForResponse(SOCKET targetedSocket, char* buffer, int bufferSize) = 0;
    virtual void Close() = 0;
};

class Socket : public ISocket {
protected:
    SOCKET currentSocket;
    sockaddr_in currentAddress;

public:
    void Close() override;
    void Send(SOCKET targetedSocket, char* buffer, int buffersize) override;
    void WaitForResponse(SOCKET targetedSocket, char* buffer, int bufferSize) override;
    Socket(int address);
    virtual ~Socket();
};

class ServerSocket : public Socket {
public:
    ServerSocket(int address);

    SOCKET WaitForConnection();
    void Connect() override;
};

class ClientSocket : public Socket {
public:
    ClientSocket(int address);

    void Connect() override;
};

void PrintHello();
