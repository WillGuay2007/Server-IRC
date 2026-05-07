#include "BaseClient.h"
#include "Channel.h"
#include "ServerUtils.h"

bool BaseClient::HasNickAndUser() {
    return (m_nick != "*" && m_username != "*" && m_realName != "*");
}

bool BaseClient::CanRegister() {
    return (HasNickAndUser() == true && m_hasRegisteredOnce == false);
}

void BaseClient::Register() {
    m_hasRegisteredOnce = true;
    Send(GeneratePrefix(RPL_WELCOME) + GetNick() + " :Welcome to the " + SERVER_NAME + " Network, " + GetNick() + "!\n");
    Send(GeneratePrefix(RPL_YOURHOST) + GetNick() + " :Your host is " + SERVER_NAME + " running version " + SERVER_VERSION + "\n");
    Send(GeneratePrefix(RPL_CREATED) + GetNick() + " :This server was created on " + SERVER_START_TIME + "\n");
    Send(GeneratePrefix(RPL_MYINFO) + GetNick() + " " + SERVER_NAME + " " + SERVER_VERSION + "\n");
}

bool BaseClient::AddChannel(Channel* channel) {
    for (int i = 0; i < m_channels.size(); i++) {
        if (m_channels[i] == channel) return false;
    }
    m_channels.push_back(channel);
    return true;
}

bool BaseClient::operator==(const BaseClient& other) const {
    return other.GetNick() == GetNick();
}