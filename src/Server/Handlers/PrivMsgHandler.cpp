#include "PrivMsgHandler.h"
#include "Channel.h"

void PrivMsgHandler::Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) {

    if (params.size() < 2) {
        clientToHandle.Send(GeneratePrefix(ERR_NEEDMOREPARAMS) + clientToHandle.GetNick() + " PRIVMSG " + ":Not enough parameters\n");
        return;
    }

    std::string messageToSend = params[params.size() - 1];
    std::string targets = params[0];
    std::vector<std::string> targetsVector;

    size_t pos = 0;
    while ((pos = targets.find(',')) != std::string::npos)
    {
        targetsVector.push_back(targets.substr(0, pos));
        targets.erase(0, pos + 1);
    }
    targetsVector.push_back(targets);

    for (std::string target : targetsVector) {
        BaseClient* client = m_clientRegistry.FindClientByNick(target);
        Channel* channel = m_channelRegistry.FindChannelByName(target);

        if (channel != nullptr) {
            channel->NotifyMembers(messageToSend + "\n");
            continue;
        }

        if (client != nullptr) {
            client->Send(messageToSend + "\n");
            continue;
        }
    }

}