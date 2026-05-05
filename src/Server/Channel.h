#include <string>
#include "BaseClient.h"
#include <vector>
#include <iostream>
#pragma once

class Channel {
public:
    Channel(std::string channelName) : m_name(channelName){}
    std::string GetName() {return m_name;}
    std::vector<BaseClient*> GetMembers() {return m_members;}
    bool HasMember(BaseClient* memberToCheck);
    void AddMember(BaseClient* member) {m_members.push_back(member); std::cout << "Added member on channel " << m_name << std::endl;}
    void NotifyMembers(std::string message);
private:
    std::string m_name;
    std::vector<BaseClient*> m_members;
};