#pragma once
#include <unordered_map>
#include <string>
#include "handler.h"

class ClientRegistry;
class Channel;
class ServerClient; //TODO: Make the client handler work with any subclass of BaseClient
class CommandDispatcher;

class ClientHandler {
public:
    ClientHandler(
        ServerClient& client,
        ClientRegistry& registry,
        std::vector<Channel*>& channels,
        CommandDispatcher& commandDispatcher
        )
        : m_client(client), m_registry(registry), m_channels(channels), m_commandDispatcher(commandDispatcher) {}
    void Handle();
private:
    ServerClient& m_client;
    ClientRegistry& m_registry;
    std::vector<Channel*>& m_channels;
    CommandDispatcher& m_commandDispatcher;
};