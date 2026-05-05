#pragma once
#include <string>
#include <vector>
#include "BaseClient.h"

class ClientSocket;

class ServerClient : public BaseClient {
public:
    ServerClient(ClientSocket* socket);
    ~ServerClient();

    ClientSocket* GetSocket() const {return m_socket;}

    bool Receive(char* buffer, int size);
    void Send(const std::string response) override;

private:
    ClientSocket* m_socket;
};