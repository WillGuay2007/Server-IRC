#include "NoticeHandler.h"
#include "Channel.h"

//Code dupliqué avec privMsg, j'aurai pu refactor mais pas ma priorité
void NoticeHandler::Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) {

    if (clientToHandle.HasNickAndUser() == false) {
        clientToHandle.Send(GeneratePrefix(ERR_NOTREGISTERED) + " " + clientToHandle.GetNick() + " Must register first with USER and NICK before sending any message.");
        return;
    }

    if (params.size() < 2) {
        clientToHandle.Send(GeneratePrefix(ERR_NEEDMOREPARAMS) + clientToHandle.GetNick() + " NOTICE " + ":Not enough parameters\r\n");
        return;
    }

    std::string targets = params[0];
    std::string messageToSend = params[1];
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
            if (channel->HasMember(&clientToHandle)) {
                channel->NotifyMembers(":" + clientToHandle.GetNick() + " NOTICE " + target + " :" + messageToSend + "\r\n", &clientToHandle);
            }
            continue;
        }

        if (client != nullptr) {
            client->Send(":" + clientToHandle.GetNick() + " NOTICE " + target + " :" + messageToSend + "\r\n");
            continue;
        }

        clientToHandle.Send(GeneratePrefix(ERR_NOSUCHNICK) + clientToHandle.GetNick() + " " + target + " :There was no such nickname.\r\n");
        
    }

}