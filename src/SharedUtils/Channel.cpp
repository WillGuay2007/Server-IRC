#include "Channel.h"

void Channel::NotifyMembers(std::string message) {
    for (int i = 0; i < m_members.size(); i++) {
        m_members[i]->BroadcastNotify(message);
    }
}