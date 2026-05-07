#include "NamesHandler.h"
#include "ChannelRegistry.h"
#include "Channel.h"

void NamesHandler::Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) {

    if (params.size() < 1) {
        clientToHandle.Send(GeneratePrefix(ERR_NEEDMOREPARAMS) + clientToHandle.GetNick() + " NAMES " + ":Not enough parameters\n");
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
    }

}