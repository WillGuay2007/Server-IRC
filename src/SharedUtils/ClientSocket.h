#pragma once
#include "Socket.h"

class ClientSocket : public Socket {
public:
    ClientSocket(int address);
    ClientSocket(void* windowSocket);

    void Connect();
};