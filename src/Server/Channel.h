#include <string>
#include "ServerClient.h"
#include <vector>
#include <iostream>
#pragma once

class Channel {
public:
    Channel(std::string channelName) : m_name(channelName){}
    std::string GetName() {return m_name;}
    std::vector<ServerClient*> GetMembers() {return m_members;}
    void AddMember(ServerClient* member) {m_members.push_back(member); std::cout << "Added member on channel " << m_name << std::endl;}
    void NotifyMembers(std::string message);
private:
    std::string m_name;
    std::vector<ServerClient*> m_members;
};