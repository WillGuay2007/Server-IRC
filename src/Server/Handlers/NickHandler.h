#pragma once

#include "handler.h"
#include "ClientRegistry.h"
#include <vector>

class ClientRegistry;

class NickHandler : public Handler {
    public:
        void Handle(const std::vector<std::string>& params) override;
        NickHandler(BaseClient& _client, ClientRegistry& clients) : Handler(_client), m_clients(clients) {}
        ~NickHandler() = default;
    private:
    ClientRegistry& m_clients;
};