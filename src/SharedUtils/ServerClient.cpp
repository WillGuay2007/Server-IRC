#include "ServerClient.h"
#include "ClientSocket.h"
#include "Channel.h"

ServerClient::ServerClient(ClientSocket* socket) : m_socket(socket) {}
ServerClient::~ServerClient() { delete m_socket;}

bool ServerClient::AddChannel(Channel* channel) {
    for (int i = 0; i < m_channels.size(); i++) {
        if (m_channels[i] == channel) return false;
    }
    m_channels.push_back(channel);
    return true;
}

void ServerClient::BroadcastNotify(const std::string& message) {
    m_socket->Send(message.c_str(), message.size());
}

bool ServerClient::IsInChannel(std::string channelName) {
    for (int i = 0; i < m_channels.size(); i++) {
        if (m_channels[i]->GetName() == channelName) return true;
    }
    return false;
}