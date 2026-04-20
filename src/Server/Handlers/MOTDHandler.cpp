#include "MOTDHandler.h"
#include "ServerClient.h"

void MOTDHandler::Handle(const std::vector<std::string>& params) {
    if (!MOTD.empty()) {
        client.Send(MOTD);
        return;
    } else {
        client.Send(GeneratePrefix(ERR_NOMOTD) + ":The server does not have a message of the day.\n");
    }
}
