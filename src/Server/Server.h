#pragma once

#include "Channel.h"
#include "ClientRegistry.h"

class Server {
public:
    void Start();
private:
    std::vector<Channel*> m_channels {
        new Channel("#General")
    };
    ClientRegistry m_clientRegistry;
};