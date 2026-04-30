#pragma once

#include "handler.h"
#include <vector>

class MOTDHandler : public Handler {
    public:
        ~MOTDHandler() = default;
        void Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) override;
};