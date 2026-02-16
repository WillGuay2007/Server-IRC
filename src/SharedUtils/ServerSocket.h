#pragma once
#include "ClientSocket.h"

class ServerSocket : public Socket {
public:
    ServerSocket(int address);
    ServerSocket(void* windowSocket);

    ClientSocket WaitForConnection();
    void StartListening();
};