#pragma once
#include "Socket.h"

class ClientSocket : public Socket {
public:
    ClientSocket(int port, char* ipAddress = (char*)"127.0.0.1");
    ClientSocket(void* windowSocket);

    void Send(const char* buffer, int buffersize);
    bool WaitForResponse(char* buffer, int buffersize);
    void Connect();
    void Disconnect();
};