#include "CommandDispatcher.h"
#include "IrcMessage.h"
#include "ServerClient.h"
#include <string>
#include <vector>

#include "handler.h"

CommandDispatcher::~CommandDispatcher() {
    for (auto& pair : m_handlers) {
        delete pair.second;
    }
}

bool CommandDispatcher::Dispatch(const IrcMessage& msg, BaseClient& clientToHandle) {
    if (m_handlers.count(msg.GetCommand())) {
        m_handlers[msg.GetCommand()]->Handle(msg.GetParams(), clientToHandle);
        return true;
    } 
    return false;
}