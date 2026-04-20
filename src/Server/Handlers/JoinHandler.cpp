#include "JoinHandler.h"
#include "ServerClient.h"
#include "Channel.h"

void JoinHandler::Handle(const std::vector<std::string>& params) {
    if (params.empty()) {
        client.Send(GeneratePrefix(ERR_NEEDMOREPARAMS) + client.GetNick() + " JOIN " + ":Not enough parameters\n");
        return;
    }
    std::string channelName = params[0];
    if (client.IsInChannel(channelName)) {
        client.Send("You are already a member of channel: " + channelName +  "\n");
        return;
    }
    for (int i = 0; i < m_channels.size(); i++) {
        if (m_channels[i]->GetName() == channelName) {
            m_channels[i]->AddMember(&client);
            client.AddChannel(m_channels[i]);
            client.Send("Executing command: JOIN\nJoining " + channelName + " channel\n");
            return;
        }
    }
    client.Send("Channel " + params[0] + " is invalid.\n");
}
