#pragma once

#include "handler.h"
#include <vector>

class ServerClient;

class PingHandler : public Handler {
    public:
        PingHandler() = default;
        ~PingHandler() = default;
        std::string Handle(const std::vector<std::string>& params) override;
    private:
};