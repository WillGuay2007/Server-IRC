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
    std::string command = msg.GetCommand();
    if (m_handlers.count(command)) {
        m_handlers[msg.GetCommand()]->Handle(msg.GetParams(), clientToHandle);
        //std::cout << "Executing command: " + command + "\n";
        return true;
    } 
    return false;
}