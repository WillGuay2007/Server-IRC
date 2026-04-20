#pragma once

#include "handler.h"
#include <vector>

class UserHandler : public Handler {
    public:
        void Handle(const std::vector<std::string>& params) override;
        UserHandler(BaseClient& _client) : Handler(_client) {};
        ~UserHandler() = default;
};