#include "PingHandler.h"
#include "ServerClient.h"

void PingHandler::Handle(const std::vector<std::string>& params) {
    if (params.empty()) {
        client.Send("PONG\n");
        return;
    }
    client.Send("PONG " + params[0] + "\n");
}   