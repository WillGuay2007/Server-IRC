#include "Channel.h"

void Channel::RemoveMember(BaseClient* memberToRemove) {
    for (auto it = m_members.begin(); it != m_members.end(); it++) {
        if (*it == memberToRemove) {
            m_members.erase(it);
            memberToRemove->RemoveChannel(this);
            return;
        }
    }
}

bool Channel::HasMember(BaseClient* memberToCheck) {
    for (BaseClient* member : m_members) {
        if (memberToCheck == member) return true;
    }
    return false;
}

std::string Channel::GetNicksForNamReply() {
    std::string nicks;
    for (BaseClient* member : m_members) {
        nicks += member->GetNick() + " ";
    }
    return nicks;
}