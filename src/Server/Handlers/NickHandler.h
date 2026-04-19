#pragma once

#include "handler.h"
#include "ClientRegistry.h"
#include <vector>

class ServerClient;
class ClientRegistry;

class NickHandler : public Handler {
    public:
        NickHandler(ServerClient& client, ClientRegistry& clients) : m_client(client), m_clients(clients) {}
        std::string Handle(const std::vector<std::string>& params) override;
        ~NickHandler() = default;
    private:
    ServerClient& m_client;
    ClientRegistry& m_clients;
};