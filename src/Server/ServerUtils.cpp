#include "ServerUtils.h"
#include "ServerConfigs.h"

std::string GeneratePrefix(EServerResponse response) {
    return ":" + SERVER_NAME + " " + std::to_string((int)response) + " ";
}