#pragma once

#include <vector>
#include <string>

class BaseClient;

class ClientRegistry {
public:
    void Add(BaseClient* client);
    void Remove(BaseClient* client);
    BaseClient* FindClientByNick(const std::string nick);
    void BroadcastMessage(const std::string message);
private:
    std::vector<BaseClient*> m_clients;
};