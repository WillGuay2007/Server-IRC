#pragma once

#include "handler.h"
#include <vector>

class MOTDHandler : public Handler {
    public:
        MOTDHandler(BaseClient& _client) : Handler(_client) {}
        ~MOTDHandler() = default;
        void Handle(const std::vector<std::string>& params) override;
};