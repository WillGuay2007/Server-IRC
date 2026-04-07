#include <winsock2.h>
#include <iostream>
#include "Socket.h"

struct SocketImpl {
    SOCKET socket;
    sockaddr_in port;   
    char* ipAddress; 
};

Socket::Socket(int port, char* ipAddress) {
    m_impl = new SocketImpl;
    m_impl->socket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port); // port IRC
    m_impl->port = addr;
    m_impl->ipAddress = ipAddress;
}

Socket::Socket(void* windowSocket) {
    m_impl = new SocketImpl;
    m_impl->socket = *(SOCKET*)windowSocket;
    //Pas besoin de mettre l'adresse ici puisque le socket la contient deja.
}

Socket::~Socket() {
    std::cout << "Closing socket\n";
    closesocket(m_impl->socket);
    delete m_impl;
    m_impl = nullptr;
}

void* Socket::GetWindowSocket() {
    return &m_impl->socket;
}

void* Socket::GetPort() {
    return &m_impl->port;
}

char* Socket::GetIpAddress() {
    return m_impl->ipAddress;
}
