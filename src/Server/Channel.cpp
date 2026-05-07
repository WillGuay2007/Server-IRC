#include "Channel.h"

void Channel::RemoveMember(BaseClient* memberToRemove) {
    for ( BaseClient* member : m_members) {
        for (auto it = m_members.begin(); it != m_members.end(); it++) {
            if (*it == memberToRemove) {
                m_members.erase(it);
                return;
            }
        }
    }
}

bool Channel::HasMember(BaseClient* memberToCheck) {
    for (BaseClient* member : m_members) {
        if (memberToCheck == member) return true;
    }
    return false;
}