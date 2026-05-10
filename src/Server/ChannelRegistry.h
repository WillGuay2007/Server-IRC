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
    std::vector<Channel*> GetChannels() { return m_channels; }
private:
    std::vector<Channel*> m_channels;
};