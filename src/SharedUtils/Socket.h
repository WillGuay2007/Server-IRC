#pragma once

struct SocketImpl;

class Socket {
private:
    inline static int s_winsockCount = 0;
    SocketImpl* m_impl;
protected:
    void* GetWindowSocket();
    void* GetPort();
    char* GetIpAddress();
public:
    Socket(int address, char* ipAddress = nullptr);
    Socket(void* windowSocket);
    virtual ~Socket();
};