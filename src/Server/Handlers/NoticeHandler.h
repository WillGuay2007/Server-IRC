#pragma once

#include "handler.h"
#include "ClientRegistry.h"
#include "ChannelRegistry.h"
#include <vector>

//Code dupliqué avec privMsg, j'aurai pu refactor mais pas ma priorité
class NoticeHandler : public Handler {
public:
    NoticeHandler(ClientRegistry& clientRegistry, ChannelRegistry& channelRegistry)
     : m_clientRegistry(clientRegistry),
       m_channelRegistry(channelRegistry) {}
    ~NoticeHandler() = default;
    void Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) override;
private:
    ClientRegistry& m_clientRegistry;
    ChannelRegistry& m_channelRegistry;
};