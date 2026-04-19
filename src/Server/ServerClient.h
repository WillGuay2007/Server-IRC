#pragma once
#include <string>
#include <vector>

class ClientSocket;
class Channel;

class ServerClient {
public:
    ServerClient(ClientSocket* socket);
    ~ServerClient();

    ClientSocket* GetSocket() const {return m_socket;}
    bool IsInChannel(std::string channelName);
    bool CheckIfIsRegistered();
    const std::string GetNick() const { return m_nick;}
    const std::string& GetUsername() const { return m_username; }
    const std::string& GetRealName() const { return m_realName; }

    void SetNick(const std::string& nick) { m_nick = nick; }
    void SetUsername(const std::string& username) { m_username = username; }
    void SetRealName(const std::string& realName) { m_realName = realName; }
    void BroadcastNotify(const std::string& message);
    bool AddChannel(Channel* channel);

    bool operator==(const ServerClient& other) const;

    bool Receive(char* buffer, int size);
    void Send(std::string response);

private:
    ClientSocket* m_socket;
    std::string m_nick = "*";
    std::string m_username = "*";
    std::string m_realName = "*";
    std::vector<Channel*> m_channels;
};