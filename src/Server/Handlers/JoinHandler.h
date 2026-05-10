#pragma once

#include "handler.h"
#include <vector>

#include "ChannelRegistry.h"

class JoinHandler : public Handler {
    public:
        void Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) override;
        JoinHandler(ChannelRegistry& channelRegistry) : m_channelRegistry(channelRegistry) {}
        ~JoinHandler() = default;
    private:
    ChannelRegistry& m_channelRegistry;
};