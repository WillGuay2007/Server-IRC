#include "PartHandler.h"
#include "Channel.h"
#include "ChannelRegistry.h"

void PartHandler::Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) {

    if (params.size() < 1) {
        clientToHandle.Send(GeneratePrefix(ERR_NEEDMOREPARAMS) + clientToHandle.GetNick() + " PART :Not enough parameters\n");
        return;
    }

    std::string channelsString = params[0];
    std::string leaveReason = params.size() > 1 ? params[1] : "Leaving";
    std::vector<std::string> channelStringsVector;

    size_t pos = 0;
    while ((pos = channelsString.find(',')) != std::string::npos)
    {
        channelStringsVector.push_back(channelsString.substr(0, pos));
        channelsString.erase(0, pos + 1);
    }
    channelStringsVector.push_back(channelsString);

    for (std::string channelName : channelStringsVector) {
        Channel* foundChannel = m_channelRegistry.FindChannelByName(channelName);
        if (foundChannel != nullptr) {
            if (foundChannel->HasMember(&clientToHandle)) {
                foundChannel->RemoveMember(&clientToHandle);
                foundChannel->NotifyMembers(":" + clientToHandle.GetNick() + "!" + clientToHandle.GetUsername() + " PART " + channelName + " :" + leaveReason + "\n", &clientToHandle);
                clientToHandle.Send(":" + clientToHandle.GetNick() + "!" + clientToHandle.GetUsername() + " PART " + channelName + " :" + leaveReason + "\n");
                continue;
            } else {
                clientToHandle.Send(GeneratePrefix(ERR_NOTONCHANNEL) + clientToHandle.GetNick() + " " + channelName + " :You're not on that channel\n");
                continue;
            }
           
        } else {
            clientToHandle.Send(GeneratePrefix(ERR_NOSUCHCHANNEL) + clientToHandle.GetNick() + " " + channelName + " :No such channel\n");
        }
    }

}