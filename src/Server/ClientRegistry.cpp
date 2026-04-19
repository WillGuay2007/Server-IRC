#include "ClientRegistry.h"
#include "ServerClient.h"

ServerClient* ClientRegistry::FindClientByNick(std::string nick) {
    for (ServerClient* client : m_clients) {
        if (client->GetNick() == nick) return client;
    }
    return nullptr;
}

void ClientRegistry::Add(ServerClient* client){
    m_clients.push_back(client);
};

void ClientRegistry::Remove(ServerClient* client) {
    for (auto it = m_clients.begin(); it != m_clients.end(); it++) {
        if (*it == client) {
            m_clients.erase(it);
            return;
        }
    }
}