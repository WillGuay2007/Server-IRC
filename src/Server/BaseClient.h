#pragma once

#include <string>
#include <vector>

class ClientSocket;
class Channel;

class BaseClient {
public:
    BaseClient() = default;
    virtual ~BaseClient() = default;

    bool IsInChannel(std::string channelName);
    bool CheckIfIsRegistered();
    const std::string GetNick() const { return m_nick;}
    const std::string& GetUsername() const { return m_username; }
    const std::string& GetRealName() const { return m_realName; }

    void SetNick(const std::string& nick) { m_nick = nick; }
    void SetUsername(const std::string& username) { m_username = username; }
    void SetRealName(const std::string& realName) { m_realName = realName; }
    bool AddChannel(Channel* channel);

    virtual void Send(const std::string response) = 0;

private:
    std::string m_nick = "*";
    std::string m_username = "*";
    std::string m_realName = "*";
    std::vector<Channel*> m_channels;
};