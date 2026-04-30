#include "MOTDHandler.h"
#include "ServerClient.h"

void MOTDHandler::Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) {
    if (!MOTD.empty()) {
        clientToHandle.Send(MOTD);
        return;
    } else {
        clientToHandle.Send(GeneratePrefix(ERR_NOMOTD) + ":The server does not have a message of the day.\n");
    }
}
