#pragma once

#include "handler.h"
#include <vector>

class Channel;

class JoinHandler : public Handler {
    public:
        void Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) override;
        JoinHandler(std::vector<Channel*>& channels) : m_channels(channels) {}
        ~JoinHandler() = default;
    private:
    std::vector<Channel*>& m_channels;
};