#include "handler.h"

std::string Handler::GeneratePrefix(EServerResponse response) {
    return ":" + SERVER_NAME + " " + std::to_string((int)response) + " ";
}