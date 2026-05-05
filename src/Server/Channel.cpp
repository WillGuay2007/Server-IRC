#include "Channel.h"

void Channel::NotifyMembers(std::string message) {
    for (int i = 0; i < m_members.size(); i++) {
        m_members[i]->Send(message);
    }
}

bool Channel::HasMember(BaseClient* memberToCheck) {
    for (BaseClient* member : m_members) {
        if (memberToCheck == member) return true;
    }
    return false;
}