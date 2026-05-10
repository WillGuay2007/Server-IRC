#include "TopicHandler.h"
#include "Channel.h"
#include "ChannelRegistry.h"

void TopicHandler::Handle(const std::vector<std::string> &params, BaseClient &clientToHandle) {
    if (params.empty()) {
        clientToHandle.Send(GeneratePrefix(ERR_NEEDMOREPARAMS) + clientToHandle.GetNick() + " TOPIC " + ":Not enough parameters\r\n");
        return;
    }

    Channel* channel = m_channelRegistry.FindChannelByName(params[0]);
    if (channel == nullptr) return;

    if (params.size() == 1) {
        if (channel->GetTopic() == "") {
            clientToHandle.Send(GeneratePrefix(RPL_NOTOPIC) + clientToHandle.GetNick() + " " + channel->GetName() + " :Channel has no topic.\r\n");
        }
        else clientToHandle.Send(GeneratePrefix(RPL_TOPIC) + clientToHandle.GetNick() + " " + channel->GetName() + " :" + channel->GetTopic() + "\r\n"); 
        return;
    } 
    else if (params.size() == 2) {
        if (!channel->HasMember(&clientToHandle)) {
            clientToHandle.Send(GeneratePrefix(ERR_NOTONCHANNEL) + clientToHandle.GetNick() + " " + channel->GetName() + " :You're not on that channel\r\n");
            return;
        }
        channel->SetTopic(params[1]);
        clientToHandle.Send(GeneratePrefix(RPL_TOPIC) + clientToHandle.GetNick() + " " + channel->GetName() + " :" + channel->GetTopic() + "\r\n"); 
        channel->NotifyMembers(":" + clientToHandle.GetNick() + " TOPIC " + channel->GetName() + " :" + channel->GetTopic() + "\r\n", &clientToHandle);
    }
}
