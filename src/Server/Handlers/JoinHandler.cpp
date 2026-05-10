#include "JoinHandler.h"
#include "ServerClient.h"
#include "Channel.h"

//C'etais pas marqué dans la grille de sauvegarder les conversations.
void JoinHandler::Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) {
    if (params.empty()) {
        clientToHandle.Send(GeneratePrefix(ERR_NEEDMOREPARAMS) + clientToHandle.GetNick() + " JOIN " + ":Not enough parameters\r\n");
        return;
    }

    std::string channelName = params[0];
    Channel* desiredChannel = m_channelRegistry.FindChannelByName(channelName);

    if (desiredChannel == nullptr) return;
    if (desiredChannel->HasMember(&clientToHandle)) return;

    desiredChannel->AddMember(&clientToHandle);
    clientToHandle.AddChannel(desiredChannel);
    std::string nicks = desiredChannel->GetNicksForNamReply();
    clientToHandle.Send(":" + clientToHandle.GetNick() + "!" + clientToHandle.GetUsername() + " JOIN " + channelName + "\r\n");
    if (desiredChannel->GetTopic() != "" ) clientToHandle.Send(GeneratePrefix(RPL_TOPIC) + clientToHandle.GetNick() + " " + desiredChannel->GetName() + " :" + desiredChannel->GetTopic() + "\r\n");
    clientToHandle.Send(GeneratePrefix(RPL_NAMREPLY) + clientToHandle.GetNick() + " = " + desiredChannel->GetName() + " :" + nicks + "\r\n");
    clientToHandle.Send(GeneratePrefix(RPL_ENDOFNAMES) + clientToHandle.GetNick() + " " + desiredChannel->GetName() + " :End of NAMES list\r\n");
    desiredChannel->NotifyMembers(":" + clientToHandle.GetNick() + "!" + clientToHandle.GetUsername() + " JOIN " + channelName + "\r\n", &clientToHandle);
}
