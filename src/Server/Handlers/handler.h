#include <iostream>
#include <vector>
#include "ServerResponses.h"
#include "ServerConfigs.h"

#pragma once

class Handler {
    public:
    virtual std::string Handle(std::vector<std::string>& params) = 0;
    virtual ~Handler() = default;
    protected:
    std::string GeneratePrefix(EServerResponse response);
};