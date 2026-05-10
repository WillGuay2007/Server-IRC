#pragma once

#include "handler.h"

class OperHandler : public Handler {
public:
    OperHandler() = default;
    ~OperHandler() = default;
    void Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) override;
};