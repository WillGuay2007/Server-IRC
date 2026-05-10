#pragma once

#include "handler.h"

class ChannelRegistry;

class PartHandler : public Handler {
public:
    void Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) override;
    PartHandler(ChannelRegistry& channelRegistry) : m_channelRegistry(channelRegistry) {}
    ~PartHandler() = default;
private:
    ChannelRegistry& m_channelRegistry;
};