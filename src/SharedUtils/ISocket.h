#pragma once

class ISocket {
public:
    virtual void Send(const char* buffer, int buffersize) = 0;
    virtual bool WaitForResponse(char* buffer, int buffersize) = 0;
};