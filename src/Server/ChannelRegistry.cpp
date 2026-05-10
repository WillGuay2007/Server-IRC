#include "ChannelRegistry.h"
#include "Channel.h"
#include "BaseClient.h"

Channel* ChannelRegistry::FindChannelByName(std::string channelName) {
    for (Channel* c : m_channels) if (c->GetName() == channelName) return c;
    return nullptr;
}