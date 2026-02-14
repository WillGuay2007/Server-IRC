#pragma once
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <iostream>

class ClientSocket;

class ISocket {
public:
    virtual void Send(const char* buffer, int buffersize) = 0;
    virtual void WaitForResponse(char* buffer, int buffersize) = 0;
};

class Socket : public ISocket {
protected:
    SOCKET currentSocket;
    sockaddr_in currentAddress;

public:
    void Send(const char* buffer, int buffersize) override;
    void WaitForResponse(char* buffer, int buffersize) override;
    Socket(int address);
    Socket(SOCKET socket);
    virtual ~Socket();
};

class ServerSocket : public Socket {
public:
    ServerSocket(int address);
    ServerSocket(SOCKET socket);

    ClientSocket WaitForConnection();
    void StartListening();
};

class ClientSocket : public Socket {
public:
    ClientSocket(int address);
    ClientSocket(SOCKET socket);

    void Connect();
};

void PrintHello();
