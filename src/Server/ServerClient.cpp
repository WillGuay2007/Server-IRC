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

bool ServerClient::CheckIfIsRegistered() {
    return (m_nick != "*" && m_username != "*" && m_realName != "*");
}

bool ServerClient::operator==(const ServerClient& other) const {
    return other.GetSocket() == this->GetSocket();
}

bool ServerClient::Receive(char* buffer, int size) { return m_socket->WaitForResponse(buffer, size); }
void ServerClient::Send(std::string response) {m_socket->Send(response.c_str(), response.size());}