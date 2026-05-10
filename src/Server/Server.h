#pragma once

#include "Channel.h"
#include "ClientRegistry.h"
#include "ChannelRegistry.h"
#include "CommandDispatcher.h"

class Server {
public:
    Server() : m_commandDispatcher(CreateCommandDispatcher()) {}
    void Start();
    ChannelRegistry& GetChannelRegistry() { return m_channelRegistry; }
    ClientRegistry& GetClientRegistry() { return m_clientRegistry; }
    int GetClientCount() { return m_clientRegistry.GetClients().size(); }
    int GetMaxClientCount() { return m_maxNumberOfClients; }
    void SetMaxNumberOfClients(int newNumber) { m_maxNumberOfClients = newNumber;}
private:
    CommandDispatcher CreateCommandDispatcher();
    ChannelRegistry m_channelRegistry {
        new Channel("#General", "Have fun and talk!"),
        new Channel("#SigmaChannel", "Idk man, just talk...")
    };
    ClientRegistry m_clientRegistry;
    CommandDispatcher m_commandDispatcher;
    int m_maxNumberOfClients = 25;
};