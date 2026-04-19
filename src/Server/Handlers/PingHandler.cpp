#include "PingHandler.h"
#include "ServerClient.h"

std::string PingHandler::Handle(const std::vector<std::string>& params) {
    if (params.empty()) {
        return "PONG\n";
    }
    return "PONG " + params[0] + "\n";
}   