#pragma once

#include "handler.h"
#include <vector>

class UserHandler : public Handler {
    public:
        void Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) override;
        ~UserHandler() = default;
};