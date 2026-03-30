#pragma once
#include <string>
#include <vector>

class ClientSocket;
class Channel;

class ServerClient {
public:
    ServerClient(ClientSocket* socket);
    ServerClient() {};
    ~ServerClient();

    ClientSocket* GetSocket() const {return m_socket;}
    bool IsInChannel(std::string channelName);
    const std::string GetNick() const { return m_nick != "" ? m_nick : std::string("*");}
    const std::string& GetUsername() const { return m_username; }
    const std::string& GetRealName() const { return m_realName; }

    void SetNick(const std::string& nick) { m_nick = nick; }
    void SetUsername(const std::string& username) { m_username = username; }
    void SetRealName(const std::string& realName) { m_realName = realName; }
    void BroadcastNotify(const std::string& message);
    bool AddChannel(Channel* channel);

private:
    ClientSocket* m_socket;
    std::string m_nick = "";
    std::string m_username = "";
    std::string m_realName = "";
    std::vector<Channel*> m_channels;
};