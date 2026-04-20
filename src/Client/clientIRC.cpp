#include "clientIRC.h"

ClientIRC::ClientIRC(ClientSocket* clientSocket)
{
    client = clientSocket;
    nickname = "Guest";
}

// Extraire pseudo
std::string ClientIRC::ExtractNickname(const std::string& ircMessage)
{
    if (!ircMessage.empty() && ircMessage[0] == ':')
    {
        size_t end = ircMessage.find('!');
        if (end != std::string::npos)
        {
            return ircMessage.substr(1, end - 1);
        }
    }
    return "Unknown";
}

// Extraire message
std::string ClientIRC::ExtractMessage(const std::string& ircMessage)
{
    size_t pos = ircMessage.find(" :");
    if (pos != std::string::npos)
    {
        return ircMessage.substr(pos + 2);
    }
    return ircMessage;
}

void ClientIRC::HandleIRCMessage(const std::string& ircMessage)
{
    // WELCOME
    if (ircMessage.find(" 001 ") != std::string::npos)
    {
        size_t start = ircMessage.find(" 001 ") + 5;
        size_t end = ircMessage.find(" :", start);

        if (end != std::string::npos)
        {
            nickname = ircMessage.substr(start, end - start);
            std::lock_guard<std::mutex> lock(messagesMutex);
            messages.push_back("[INFO] Your nickname is " + nickname);
        }
        return;
    }

    // PONG
    if (ircMessage.rfind("PING", 0) == 0)
    {
        std::string response = "PONG" + ircMessage.substr(4) + "\r\n";
        client->Send(response.c_str(), (int)response.size());
        return;
    }

    // MOTD
    if (ircMessage.find(" 372 ") != std::string::npos ||
        ircMessage.find(" 375 ") != std::string::npos ||
        ircMessage.find(" 376 ") != std::string::npos)
    {
        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[MOTD] " + ExtractMessage(ircMessage));
        return;
    }

    // LUSERS
    if (ircMessage.find(" 251 ") != std::string::npos ||
        ircMessage.find(" 252 ") != std::string::npos ||
        ircMessage.find(" 253 ") != std::string::npos ||
        ircMessage.find(" 254 ") != std::string::npos ||
        ircMessage.find(" 255 ") != std::string::npos)
    {
        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[LUSERS] " + ExtractMessage(ircMessage));
        return;
    }

    // JOIN
    if (ircMessage.find(" JOIN ") != std::string::npos)
    {
        std::string nick = ExtractNickname(ircMessage);

        size_t pos = ircMessage.find("JOIN ");
        std::string channel = (pos != std::string::npos)
            ? ircMessage.substr(pos + 5)
            : "";

        if (!channel.empty() && channel[0] == ':')
        {
            channel = channel.substr(1);
        }
        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[INFO] " + nick + " joined " + channel);
        return;
    }

    std::string nick = ExtractNickname(ircMessage);
    std::string text = ExtractMessage(ircMessage);

    std::lock_guard<std::mutex> lock(messagesMutex);
    messages.push_back(nick + ": " + text);
}

std::vector<std::string> ClientIRC::GetMessages()
{
    std::lock_guard<std::mutex> lock(messagesMutex);
    return messages;
}

std::string ClientIRC::GetNickname() const
{
    return nickname;
}