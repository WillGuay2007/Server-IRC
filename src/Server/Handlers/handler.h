#pragma once
#include <iostream>
#include <vector>
#include "ServerUtils.h"
#include "BaseClient.h"

class Handler {
public:
    virtual void Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) = 0;
    Handler() = default;
    virtual ~Handler() = default;
};