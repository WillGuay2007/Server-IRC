#pragma once
#include <string>
#include <ctime>

const std::string SERVER_NAME = "ScaryServer";
const std::string SERVER_VERSION = "1.69";
const std::string MOTD = "MOTD msg\n";

inline std::string GetServerStartTime() {
    std::time_t now = std::time(nullptr);
    return std::string(std::ctime(&now), 24);
}

const std::string SERVER_START_TIME = GetServerStartTime();