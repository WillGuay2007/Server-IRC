#pragma once

#include "handler.h"
#include <vector>

class ChannelRegistry;

class ListHandler : public Handler {
    public:
        void Handle(const std::vector<std::string> &params, BaseClient &clientToHandle) override;
        void SendList(Channel *channel, BaseClient &clientToHandle);
        ListHandler(ChannelRegistry& channelRegistry) : m_channelRegistry(channelRegistry) {}
        ~ListHandler() = default;
    private:
    ChannelRegistry& m_channelRegistry;
};