#pragma once

#include "handler.h"
#include <vector>

class AwayHandler : public Handler {
    public:
        ~AwayHandler() = default;
        void Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) override;
};