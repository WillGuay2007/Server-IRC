#pragma once
#include "Socket.h"

class ClientSocket : public Socket {
public:
    ClientSocket(int address);
    ClientSocket(void* windowSocket);

    void Send(const char* buffer, int buffersize);
    bool WaitForResponse(char* buffer, int buffersize);
    void Connect();
};