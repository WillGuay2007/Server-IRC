#pragma once

#include <vector>
#include <string>
#include <initializer_list>

class Channel;
class BaseClient;

class ChannelRegistry {
public:
    ChannelRegistry(std::initializer_list<Channel*> channelList) : m_channels(channelList) {}
    Channel* FindChannelByName(const std::string channelName);
    void AddClientToChannel(BaseClient& clientToAdd, Channel* channel);
private:
    std::vector<Channel*> m_channels;
};