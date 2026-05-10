#pragma once

#include "handler.h"
#include <vector>

class ChannelRegistry;

class TopicHandler : public Handler {
    public:
        void Handle(const std::vector<std::string> &params, BaseClient &clientToHandle) override;
        TopicHandler(ChannelRegistry& channelRegistry) : m_channelRegistry(channelRegistry) {}
        ~TopicHandler() = default;
    private:
    ChannelRegistry& m_channelRegistry;
};