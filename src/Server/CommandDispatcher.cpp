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

void CommandDispatcher::Dispatch(const IrcMessage& msg) {
    if (m_handlers.count(msg.GetCommand())) {
        m_client->Send(m_handlers[msg.GetCommand()]->Handle(msg.GetParams()));
    } else {
        m_client->Send("Command " + msg.GetCommand() + " not found.\n");
    }
}