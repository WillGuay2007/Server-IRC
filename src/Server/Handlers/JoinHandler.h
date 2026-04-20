#pragma once

#include "handler.h"
#include <vector>

class Channel;

class JoinHandler : public Handler {
    public:
        void Handle(const std::vector<std::string>& params) override;
        JoinHandler(BaseClient& _client, std::vector<Channel*>& channels) : Handler(_client), m_channels(channels) {}
        ~JoinHandler() = default;
    private:
    std::vector<Channel*>& m_channels;
};