#pragma once

#include "handler.h"
#include <vector>

class ServerClient;
class Channel;

class JoinHandler : public Handler {
    public:
        JoinHandler(ServerClient& client, std::vector<Channel*>& channels) : m_client(client), m_channels(channels) {}
        std::string Handle(std::vector<std::string>& params) override;
        ~JoinHandler() = default;
    private:
    ServerClient& m_client;
    std::vector<Channel*>& m_channels;
};