#include "JoinHandler.h"
#include "ServerClient.h"
#include "Channel.h"

void JoinHandler::Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) {
    if (params.empty()) {
        clientToHandle.Send(GeneratePrefix(ERR_NEEDMOREPARAMS) + clientToHandle.GetNick() + " JOIN " + ":Not enough parameters\n");
        return;
    }

    std::string channelName = params[0];
    Channel* desiredChannel = m_channelRegistry.FindChannelByName(channelName);

    if (desiredChannel == nullptr) return;
    if (desiredChannel->HasMember(&clientToHandle)) return;

    std::string nicks = desiredChannel->GetNicksForNamReply();
    clientToHandle.Send(":" + clientToHandle.GetNick() + " JOIN " + channelName + "\n");
    if (desiredChannel->GetTopic() != "" ) clientToHandle.Send(GeneratePrefix(RPL_TOPIC) + clientToHandle.GetNick() + " " + desiredChannel->GetName() + " :" + desiredChannel->GetTopic() + "\n");
    clientToHandle.Send(GeneratePrefix(RPL_NAMREPLY) + clientToHandle.GetNick() + " = " + desiredChannel->GetName() + " :" + nicks + "\n");
    clientToHandle.Send(GeneratePrefix(RPL_ENDOFNAMES) + clientToHandle.GetNick() + " " + desiredChannel->GetName() + " :End of NAMES list\n");
    desiredChannel->AddMember(&clientToHandle);
}
