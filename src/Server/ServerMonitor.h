#pragma once
#include <string>
#include <vector>

class Server;

class ServerMonitor{
public:
    ServerMonitor(Server& serverToDisplay) : m_serverToDisplay(serverToDisplay) {}
    ~ServerMonitor() = default;
    void Display();
private:
    Server& m_serverToDisplay;
    std::vector<std::string> m_notifications;
};