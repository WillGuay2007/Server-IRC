#pragma once

#include <string>
#include "BaseClient.h"
#include <vector>
#include <iostream>

class Channel {
public:
    Channel(std::string channelName, std::string topic = "") : m_name(channelName), m_topic(topic) {}
    std::string GetName() {return m_name;}
    std::string GetNicksForNamReply();
    std::string GetTopic() {return m_topic;}
    std::vector<BaseClient*> GetMembers() {return m_members;}
    bool HasMember(BaseClient* memberToCheck);
    void AddMember(BaseClient* member) {m_members.push_back(member);}
    void RemoveMember(BaseClient* memberToRemove);
    void NotifyMembers(std::string message, BaseClient* clientWhoSent) {for (BaseClient* member : m_members) if (member != clientWhoSent) member->Send(message);}
private:
    std::string m_topic;
    std::string m_name;
    std::vector<BaseClient*> m_members;
};