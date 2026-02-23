#pragma once

struct SocketImpl;

class Socket {
private:
    SocketImpl* m_impl;
protected:
    void* GetWindowSocket();
    void* GetAddress();

public:
    Socket(int address);
    Socket(void* windowSocket);
    virtual ~Socket();
};