#pragma once

#include "handler.h"
#include "ClientRegistry.h"
#include "ChannelRegistry.h"
#include <vector>

class PrivMsgHandler : public Handler {
public:
    PrivMsgHandler(ClientRegistry& clientRegistry, ChannelRegistry& channelRegistry)
     : m_clientRegistry(clientRegistry),
       m_channelRegistry(channelRegistry) {}
    ~PrivMsgHandler() = default;
    void Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) override;
private:
    ClientRegistry& m_clientRegistry;
    ChannelRegistry& m_channelRegistry;
};