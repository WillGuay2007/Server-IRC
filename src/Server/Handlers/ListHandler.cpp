#include "ListHandler.h"
#include "ChannelRegistry.h"
#include "Channel.h"
   
void ListHandler::Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) {
    if (params.empty()) {
        for (Channel* channel : m_channelRegistry.GetChannels()) {
            SendList(channel, clientToHandle);
        }
    } else {
        Channel* channel = m_channelRegistry.FindChannelByName(params[0]);
        if (channel != nullptr) SendList(channel, clientToHandle);
    }
    clientToHandle.Send(GeneratePrefix(RPL_LISTEND) + clientToHandle.GetNick() + " " + ":End of list\r\n");
}

void ListHandler::SendList(Channel *channel, BaseClient &clientToHandle)
{
    int memberCount = channel->GetMembers().size();
    clientToHandle.Send(GeneratePrefix(RPL_LIST) + clientToHandle.GetNick() + " " + channel->GetName() + " " + std::to_string(memberCount) + " :" + channel->GetTopic() + "\r\n");
}
