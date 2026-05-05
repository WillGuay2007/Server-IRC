#include "JoinHandler.h"
#include "ServerClient.h"
#include "Channel.h"

//TODO: Make it actually return proper IRC messages
void JoinHandler::Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) {
    if (params.empty()) {
        clientToHandle.Send(GeneratePrefix(ERR_NEEDMOREPARAMS) + clientToHandle.GetNick() + " JOIN " + ":Not enough parameters\n");
        return;
    }
    std::string channelName = params[0];
    Channel* desiredChannel = m_channelRegistry.FindChannelByName(channelName);

    if (desiredChannel == nullptr) {
        clientToHandle.Send("Invalid channel: " + channelName +  "\n");
        return;
    }

    if (desiredChannel->HasMember(&clientToHandle)) {
        clientToHandle.Send("You are already a member of channel: " + channelName +  "\n");
        return;
    }

    m_channelRegistry.AddClientToChannel(clientToHandle, desiredChannel);
    clientToHandle.Send("Joined " + channelName +  "\n");

}
