#include "ClientRegistry.h"
#include "ServerClient.h"

BaseClient* ClientRegistry::FindClientByNick(const std::string nick) {
    for (BaseClient* client : m_clients) {
        if (client->GetNick() == nick) return client;
    }
    return nullptr;
}

void ClientRegistry::Add(BaseClient* client){
    m_clients.push_back(client);
};

void ClientRegistry::Remove(BaseClient* client) {
    for (auto it = m_clients.begin(); it != m_clients.end(); it++) {
        if (*it == client) {
            m_clients.erase(it);
            return;
        }
    }
}