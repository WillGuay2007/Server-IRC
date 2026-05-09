#include "WhoHandler.h"
#include "Channel.h"
#include "ChannelRegistry.h"
#include "ClientRegistry.h"

bool SharesChannel(BaseClient* a, BaseClient* b) {
    for (Channel* channel : a->GetChannels()) {
        if (channel->HasMember(b)) return true;
    }
    return false;
}

void WhoHandler::Handle(const std::vector<std::string> &params, BaseClient &clientToHandle) {
    std::string mask = params.empty() ? "*" : params[0];
    if (params.empty()) {
        for (BaseClient* client : m_clientRegistry.GetClients()) {
            if (SharesChannel(&clientToHandle, client)) continue;
            SendWhoReply(client, clientToHandle);
        }
    } else  {
        Channel* channel = m_channelRegistry.FindChannelByName(params[0]);
        if (channel != nullptr) for (BaseClient* client : channel->GetMembers()) SendWhoReply(client, clientToHandle, channel);
    }
    clientToHandle.Send(GeneratePrefix(RPL_ENDOFWHO) + clientToHandle.GetNick() + " " + mask + " :End of WHO list\r\n");
}

void WhoHandler::SendWhoReply(BaseClient *clientInfo, BaseClient &receiver, Channel *channel)
{
    std::string flags = clientInfo->GetAwayMessage().empty() ? "H" : "G";
    if (clientInfo->IsOper()) flags += "*";
    std::string channelName = channel ? channel->GetName() : "*";
    receiver.Send(GeneratePrefix(RPL_WHOREPLY) + channelName + " " + clientInfo->GetUsername() + " test host " + SERVER_NAME + " " + clientInfo->GetNick() + " " + flags + " 0 " + " :" + clientInfo->GetRealName() + "\r\n");
}
