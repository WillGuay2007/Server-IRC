#include "QuitHandler.h"
#include "ServerClient.h"
#include "Channel.h"
#include "ClientRegistry.h"

void QuitHandler::Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) {
    //Cast parce que les mock clients n'ont pas de socket.
    ServerClient* serverClient = dynamic_cast<ServerClient*>(&clientToHandle);
    if (serverClient != nullptr) {
    std::string reason = params.empty() ? "Client Quit" : params[0];
    clientToHandle.Send(clientToHandle.GetNick() + " ERROR :Closing Link: " + reason + "\n");
        for (Channel* channel : clientToHandle.GetChannels()) {
            channel->NotifyMembers(":" + clientToHandle.GetNick() + "!" + clientToHandle.GetUsername() + "@host QUIT :" + reason + "\n", &clientToHandle);
        }
        m_clientRegistry.Remove(&clientToHandle);
        serverClient->Disconnect();
    }
}