#include "AwayHandler.h"

void AwayHandler::Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) {
    if (params.empty()) {
        clientToHandle.SetAwayMessage("");
        clientToHandle.Send(GeneratePrefix(RPL_UNAWAY) + clientToHandle.GetNick() + " :You are no longer marked as being away\r\n");
        return;
    }
    std::string text = params[0];
    if (text == "") return;
    clientToHandle.SetAwayMessage(text);
    clientToHandle.Send(GeneratePrefix(RPL_NOWAWAY) + clientToHandle.GetNick() + " :You have been marked as being away\r\n");
}
