#pragma once

#include "handler.h"
#include "ClientRegistry.h"
#include <vector>

class ClientRegistry;

class NickHandler : public Handler {
    public:
        void Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) override;
        NickHandler(ClientRegistry& clients) : m_clients(clients) {}
        ~NickHandler() = default;
    private:
    ClientRegistry& m_clients;
};