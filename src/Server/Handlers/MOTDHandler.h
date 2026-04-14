#pragma once

#include "handler.h"
#include <vector>

class ServerClient;

class MOTDHandler : public Handler {
    public:
        MOTDHandler() = default;
        ~MOTDHandler() = default;
        std::string Handle(std::vector<std::string>& params) override;
    private:
};