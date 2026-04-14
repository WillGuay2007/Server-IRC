#pragma once

#include "handler.h"
#include <vector>

class ServerClient;

class UserHandler : public Handler {
    public:
        UserHandler(ServerClient& client) : m_client(client) {};
        std::string Handle(std::vector<std::string>& params) override;
        ~UserHandler() = default;
    private:
    ServerClient& m_client;
};