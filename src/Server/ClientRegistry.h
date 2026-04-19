#pragma once

#include <vector>
#include <string>

class ServerClient;

class ClientRegistry {
public:
    void Add(ServerClient* client);
    void Remove(ServerClient* client);
    ServerClient* FindClientByNick(const std::string nick);
    void BroadcastMessage(const std::string message);
private:
    std::vector<ServerClient*> m_clients;
};