#include "NamesHandler.h"
#include "ChannelRegistry.h"
#include "Channel.h"

void NamesHandler::Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) {

    if (params.size() < 1) {
        for (Channel* channel : m_channelRegistry.GetChannels()) {
            std::string nicks = channel->GetNicksForNamReply();
            clientToHandle.Send(GeneratePrefix(RPL_NAMREPLY) + clientToHandle.GetNick() + " = " + channel->GetName() + " :" + nicks + "\r\n");
            clientToHandle.Send(GeneratePrefix(RPL_ENDOFNAMES) + clientToHandle.GetNick() + " = " + channel->GetName() + " :End of NAMES list\r\n");
        }
        return;
    }

    std::string channelsString = params[0];
    std::vector<std::string> channelStringsVector;

    size_t pos = 0;
    while ((pos = channelsString.find(',')) != std::string::npos)
    {
        channelStringsVector.push_back(channelsString.substr(0, pos));
        channelsString.erase(0, pos + 1);
    }
    channelStringsVector.push_back(channelsString);

    for (std::string channelName : channelStringsVector) {
        Channel* foundChanel = m_channelRegistry.FindChannelByName(channelName);
        if (foundChanel != nullptr) {
            std::string nicks = foundChanel->GetNicksForNamReply();
            clientToHandle.Send(GeneratePrefix(RPL_NAMREPLY) + clientToHandle.GetNick() + " = " + foundChanel->GetName() + " :" + nicks + "\r\n");
            clientToHandle.Send(GeneratePrefix(RPL_ENDOFNAMES) + clientToHandle.GetNick() + " " + foundChanel->GetName() + " :End of NAMES list\r\n");
        }
    }

}