#include "JoinHandler.h"
#include "ServerClient.h"
#include "Channel.h"

void JoinHandler::Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) {
    if (params.empty()) {
        clientToHandle.Send(GeneratePrefix(ERR_NEEDMOREPARAMS) + clientToHandle.GetNick() + " JOIN " + ":Not enough parameters\n");
        return;
    }
    std::string channelName = params[0];
    if (clientToHandle.IsInChannel(channelName)) {
        clientToHandle.Send("You are already a member of channel: " + channelName +  "\n");
        return;
    }
    for (int i = 0; i < m_channels.size(); i++) {
        if (m_channels[i]->GetName() == channelName) {
            m_channels[i]->AddMember(&clientToHandle);
            clientToHandle.AddChannel(m_channels[i]);
            clientToHandle.Send("Executing command: JOIN\nJoining " + channelName + " channel\n");
            return;
        }
    }
    clientToHandle.Send("Channel " + params[0] + " is invalid.\n");
}
