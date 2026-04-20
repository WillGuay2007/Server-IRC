#include "BaseClient.h"
#include "Channel.h"

bool BaseClient::IsInChannel(std::string channelName) {
    for (int i = 0; i < m_channels.size(); i++) {
        if (m_channels[i]->GetName() == channelName) return true;
    }
    return false;
}

bool BaseClient::CheckIfIsRegistered() {
    return (m_nick != "*" && m_username != "*" && m_realName != "*");
}

bool BaseClient::AddChannel(Channel* channel) {
    for (int i = 0; i < m_channels.size(); i++) {
        if (m_channels[i] == channel) return false;
    }
    m_channels.push_back(channel);
    return true;
}