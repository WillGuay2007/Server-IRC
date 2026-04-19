#pragma once

#include <unordered_map>
#include <string>

class Handler;
class IrcMessage;
class ServerClient;

class CommandDispatcher {
public:
    ~CommandDispatcher();
    CommandDispatcher(std::unordered_map<std::string, Handler*>& handlers, ServerClient* client) : m_handlers(handlers), m_client(client) {}
    void Dispatch(const IrcMessage& msg);
private:
    std::unordered_map<std::string, Handler*> m_handlers;
    ServerClient* m_client;
};