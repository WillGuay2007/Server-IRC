#pragma once

#include "handler.h"

class ChannelRegistry;

class NamesHandler : public Handler {
public:
    void Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) override;
    NamesHandler(ChannelRegistry& channelRegistry) : m_channelRegistry(channelRegistry) {}
    ~NamesHandler() = default;
private:
    ChannelRegistry& m_channelRegistry;
};