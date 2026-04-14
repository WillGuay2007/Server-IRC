#pragma once

#include "handler.h"
#include <vector>

class ServerClient;

class NickHandler : public Handler {
    public:
        NickHandler(ServerClient& client, std::vector<ServerClient*>& clients) : m_client(client), m_clients(clients) {}
        std::string Handle(std::vector<std::string>& params) override;
        ~NickHandler() = default;
    private:
    ServerClient& m_client;
    std::vector<ServerClient*>& m_clients;
};