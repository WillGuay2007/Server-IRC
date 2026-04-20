#pragma once

#include "handler.h"
#include <vector>

class PingHandler : public Handler {
    public:
        PingHandler(BaseClient& _client) : Handler(_client) {}
        ~PingHandler() = default;
        void Handle(const std::vector<std::string>& params) override;
    private:
};