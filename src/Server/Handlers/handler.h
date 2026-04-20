#pragma once
#include <iostream>
#include <vector>
#include "ServerResponses.h"
#include "ServerConfigs.h"
#include "BaseClient.h"


class Handler {
public:
    virtual void Handle(const std::vector<std::string>& params) = 0;
    Handler(BaseClient& client) : client(client) {}
    virtual ~Handler() = default;
    protected:
    std::string GeneratePrefix(EServerResponse response);
    BaseClient& client;
};