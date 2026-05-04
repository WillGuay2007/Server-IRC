#pragma once

#include "Channel.h"
#include "ClientRegistry.h"
#include "CommandDispatcher.h"

class Server {
public:
    Server() : m_commandDispatcher(CreateCommandDispatcher()) {}
    void Start();
private:
    CommandDispatcher CreateCommandDispatcher();
    std::vector<Channel*> m_channels {
        new Channel("#General")
    };
    ClientRegistry m_clientRegistry;
    CommandDispatcher m_commandDispatcher;
};