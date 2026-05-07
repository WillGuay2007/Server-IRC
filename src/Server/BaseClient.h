#pragma once

#include <string>
#include <vector>

class ClientSocket;
class Channel;

class BaseClient {
public:
    BaseClient() = default;
    virtual ~BaseClient() = default;

    bool HasNickAndUser();
    bool CanRegister();
    const std::string& GetNick() const { return m_nick;}
    const std::string& GetUsername() const { return m_username; }
    const std::string& GetRealName() const { return m_realName; }

    void SetNick(const std::string& nick) { m_nick = nick; }
    void SetUsername(const std::string& username) { m_username = username; }
    void SetRealName(const std::string& realName) { m_realName = realName; }
    bool AddChannel(Channel* channel);

    bool operator==(const BaseClient& other) const;

    void Register();

    virtual void Send(const std::string response) = 0;

private:
    bool m_hasRegisteredOnce = false;
    std::string m_nick = "*";
    std::string m_username = "*";
    std::string m_realName = "*";
    std::vector<Channel*> m_channels;
};