#include "PingHandler.h"
#include "ServerClient.h"

void PingHandler::Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) {
    if (params.empty()) {
        clientToHandle.Send("PONG\n");
        return;
    }
    clientToHandle.Send("PONG " + params[0] + "\n");
}   