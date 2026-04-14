#include "JoinHandler.h"
#include "ServerClient.h"
#include "Channel.h"

std::string JoinHandler::Handle(std::vector<std::string>& params) {
    if (params.empty()) {
        return GeneratePrefix(ERR_NEEDMOREPARAMS) + m_client.GetNick() + " JOIN " + ":Not enough parameters\n";
    }
    std::string channelName = params[0];
    if (m_client.IsInChannel(channelName)) {
        return "You are already a member of channel: " + channelName +  "\n";
    }
    for (int i = 0; i < m_channels.size(); i++) {
        if (m_channels[i]->GetName() == channelName) {
            m_channels[i]->AddMember(&m_client);
            m_client.AddChannel(m_channels[i]);
            return "Executing command: JOIN\nJoining " + channelName + " channel\n";
        }
    }
    return "Channel " + params[0] + " is invalid.\n";
}
