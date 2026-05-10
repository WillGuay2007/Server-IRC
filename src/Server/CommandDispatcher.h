#pragma once

#include <unordered_map>
#include <string>

class Handler;
class IrcMessage;
class BaseClient;

class CommandDispatcher {
public:
    ~CommandDispatcher();
    CommandDispatcher(std::unordered_map<std::string, Handler*>& handlers) : m_handlers(handlers) {}
    bool Dispatch(const IrcMessage& msg, BaseClient& clientToHandle);
private:
    std::unordered_map<std::string, Handler*> m_handlers;
};