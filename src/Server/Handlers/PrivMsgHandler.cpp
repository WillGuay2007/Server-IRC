#include "PrivMsgHandler.h"
#include "Channel.h"

void PrivMsgHandler::Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) {

    if (clientToHandle.HasNickAndUser() == false) {
        //Le critere 8 de la grille si j'ai bien compris
        clientToHandle.Send(GeneratePrefix(ERR_NOTREGISTERED) + " " + clientToHandle.GetNick() + " Must register first with USER and NICK before sending any message.");
        return;
    }

    if (params.size() < 2) {
        clientToHandle.Send(GeneratePrefix(ERR_NEEDMOREPARAMS) + clientToHandle.GetNick() + " PRIVMSG " + ":Not enough parameters\n");
        return;
    }

    std::string targets = params[0];
    std::string messageToSend = params[1];
    std::vector<std::string> targetsVector;

    size_t pos = 0;
    //Je vien de realiser que j'avais pas besoin de faire ca mais pg.
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
                channel->NotifyMembers(":" + clientToHandle.GetNick() + " PRIVMSG " + target + " :" + messageToSend + "\n", &clientToHandle);
            }
            else {
                clientToHandle.Send(GeneratePrefix(ERR_CANNOTSENDTOCHAN) + clientToHandle.GetNick() + " " + channel->GetName() + " :Cannot send to channel.\n");
            }
            continue;
        }

        if (client != nullptr) {
            if (client->GetAwayMessage() != "") {
                clientToHandle.Send(GeneratePrefix(RPL_AWAY) + clientToHandle.GetNick() + " " + client->GetNick() + " :" + client->GetAwayMessage() + "\r\n");
            }
            client->Send(":" + clientToHandle.GetNick() + " PRIVMSG " + target + " :" + messageToSend + "\n");
            continue;
        }

        clientToHandle.Send(GeneratePrefix(ERR_NOSUCHNICK) + clientToHandle.GetNick() + " " + target + " :There was no such nickname.\n");
        
    }

}