#pragma once

#include "Channel.h"
#include "ClientRegistry.h"
#include "ChannelRegistry.h"
#include "CommandDispatcher.h"

class Server {
public:
    Server() : m_commandDispatcher(CreateCommandDispatcher()) {}
    void Start();
private:
    CommandDispatcher CreateCommandDispatcher();
    ChannelRegistry m_channelRegistry {
        new Channel("#General", "Have fun and talk!"),
    };
    ClientRegistry m_clientRegistry;
    CommandDispatcher m_commandDispatcher;
};