#pragma once

#include "handler.h"
#include <vector>

class PingHandler : public Handler {
    public:
        ~PingHandler() = default;
        void Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) override;
    private:
};