#include "ChannelRegistry.h"
#include "Channel.h"
#include "BaseClient.h"

void ChannelRegistry::AddClientToChannel(BaseClient& clientToAdd, Channel* channel) {
    channel->AddMember(&clientToAdd);
}

Channel* ChannelRegistry::FindChannelByName(std::string channelName) {
    for (Channel* c : m_channels) if (c->GetName() == channelName) return c;
    return nullptr;
}