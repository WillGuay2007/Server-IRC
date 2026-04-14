#include "MOTDHandler.h"
#include "ServerClient.h"

std::string MOTDHandler::Handle(std::vector<std::string>& params) {
    if (!MOTD.empty()) {
        return MOTD;
    } else {
        return GeneratePrefix(ERR_NOMOTD) + ":The server does not have a message of the day.\n";
    }
}
