#pragma once

#include "handler.h"
#include <vector>

class ChannelRegistry;
class ClientRegistry;

class WhoHandler : public Handler {
    public:
        void Handle(const std::vector<std::string> &params, BaseClient &clientToHandle) override;
        void SendWhoReply(BaseClient* clientInfo, BaseClient& receiver, Channel* channel = nullptr);
        WhoHandler(ChannelRegistry& channelRegistry, ClientRegistry& clientRegistry) : m_channelRegistry(channelRegistry), m_clientRegistry(clientRegistry) {}
        ~WhoHandler() = default;
    private:
    ChannelRegistry& m_channelRegistry;
    ClientRegistry& m_clientRegistry;
};