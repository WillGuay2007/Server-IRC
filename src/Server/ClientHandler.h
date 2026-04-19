#pragma once
#include <unordered_map>
#include <string>
#include "handler.h"

class ClientRegistry;
class Channel;
class ServerClient;
class CommandDispatcher;

class ClientHandler {
public:
    ClientHandler(ServerClient& client, ClientRegistry& registry, std::vector<Channel*>& channels)
        : m_client(client), m_registry(registry), m_channels(channels) {
            InitCommandDispatcher();
        }
    ~ClientHandler();
    void Handle();
private:
    void InitCommandDispatcher();
    ServerClient& m_client;
    ClientRegistry& m_registry;
    std::vector<Channel*>& m_channels;
    CommandDispatcher* m_commandDispatcher;
};