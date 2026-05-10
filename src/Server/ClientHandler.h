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
        CommandDispatcher& commandDispatcher
        )
        : m_client(client), m_commandDispatcher(commandDispatcher) {}
    void Handle();
private:
    ServerClient& m_client;
    CommandDispatcher& m_commandDispatcher;
};