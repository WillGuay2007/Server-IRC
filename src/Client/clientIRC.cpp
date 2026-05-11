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

void ClientIRC::SendUserInput(const std::string& input)
{
    if (input.empty()) return;

    // COMMANDES
    if (input[0] == '/')
    {
        if (input.rfind("/join ", 0) == 0)
        {
            std::string channel = input.substr(6);
            channel.erase(0, channel.find_first_not_of(" "));

            if (!channel.empty() && channel[0] != '#')
            {
                channel = "#" + channel;
            }

            if (channel.empty()) return;

            std::string cmd = "JOIN " + channel + "\r\n";
            client->Send(cmd.c_str(), (int)cmd.size());

            currentChannel = channel;

            std::lock_guard<std::mutex> lock(messagesMutex);
            messages.push_back("[SYSTEM] Joining " + channel);

            return;
        }
        else if (input.rfind("/nick ", 0) == 0)
        {
            std::string newNick = input.substr(6);
            newNick.erase(0, newNick.find_first_not_of(" "));

            std::string cmd = "NICK " + newNick + "\r\n";
            client->Send(cmd.c_str(), (int)cmd.size());

            nickname = newNick;

            std::lock_guard<std::mutex> lock(messagesMutex);
            messages.push_back("[SYSTEM] Changing nickname to " + newNick);

            return;
        }
        else if (input.rfind("/part", 0) == 0)
        {
            std::string channel;

            // Si l'utilisateur écrit
            if (input.size() > 6)
            {
                channel = input.substr(6);
                channel.erase(0, channel.find_first_not_of(" "));
            }
            else
            {
                channel = currentChannel;
            }

            if (channel.empty()) return;

            std::string cmd = "PART " + channel + "\r\n";
            client->Send(cmd.c_str(), (int)cmd.size());

            std::lock_guard<std::mutex> lock(messagesMutex);
            messages.push_back("[SYSTEM] Leaving " + channel);

            return;
        }

        else if (input.rfind("/names", 0) == 0)
        {
            std::string channel;

            if (input.size() > 7)
            {
                channel = input.substr(7);
                channel.erase(0, channel.find_first_not_of(" "));
            }
            else
            {
                channel = currentChannel;
            }

            if (channel.empty())
            {
                std::lock_guard<std::mutex> lock(messagesMutex);
                messages.push_back("[ERROR] No channel specified");
                return;
            }

            std::string cmd = "NAMES " + channel + "\r\n";
            client->Send(cmd.c_str(), (int)cmd.size());

            std::lock_guard<std::mutex> lock(messagesMutex);
            messages.push_back("[SYSTEM] Requesting user list for " + channel);

            return;
        }
        else if (input == "/list")
        {
            std::string cmd = "LIST\r\n";
            client->Send(cmd.c_str(), (int)cmd.size());

            std::lock_guard<std::mutex> lock(messagesMutex);
            messages.push_back("[SYSTEM] Requesting channel list...");

            return;
        }
        else if (input.rfind("/away", 0) == 0)
        {
            std::string message;

            if (input.size() > 6)
            {
                message = input.substr(6);
                message.erase(0, message.find_first_not_of(" "));
            }

            std::string cmd;

            if (message.empty())
            {
                cmd = "AWAY\r\n"; // désactiver
            }
            else
            {
                cmd = "AWAY :" + message + "\r\n"; // activer
            }

            client->Send(cmd.c_str(), (int)cmd.size());

            std::lock_guard<std::mutex> lock(messagesMutex);

            if (message.empty())
            {
                messages.push_back("[SYSTEM] You are no longer away");
            }
            else
            {
                messages.push_back("[SYSTEM] Away: " + message);
            }
            return;
        }
        else if (input.rfind("/who", 0) == 0)
        {
            std::string target;

            if (input.size() > 5)
            {
                target = input.substr(5);
                target.erase(0, target.find_first_not_of(" "));
            }
            else
            {
                target = currentChannel;
            }

            if (target.empty())
            {
                std::lock_guard<std::mutex> lock(messagesMutex);
                messages.push_back("[ERROR] No target specified");
                return;
            }

            std::string cmd = "WHO " + target + "\r\n";
            client->Send(cmd.c_str(), (int)cmd.size());

            std::lock_guard<std::mutex> lock(messagesMutex);
            messages.push_back("[SYSTEM] WHO " + target);

            return;
        }
        else if (input.rfind("/whois", 0) == 0)
        {
            std::string target;

            if (input.size() > 7)
            {
                target = input.substr(7);
                target.erase(0, target.find_first_not_of(" "));
            }

            if (target.empty())
            {
                std::lock_guard<std::mutex> lock(messagesMutex);
                messages.push_back("[ERROR] No nickname specified");
                return;
            }

            std::string cmd = "WHOIS " + target + "\r\n";
            client->Send(cmd.c_str(), (int)cmd.size());

            std::lock_guard<std::mutex> lock(messagesMutex);
            messages.push_back("[SYSTEM] WHOIS " + target);

            return;
        }
    }

    if (currentChannel.empty())
    {
        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[ERROR] You are not in a channel");
        return;
    }

    // MESSAGE NORMAL
    std::string msg = "PRIVMSG " + currentChannel + " :" + input + "\r\n";
    client->Send(msg.c_str(), (int)msg.size());

    std::lock_guard<std::mutex> lock(messagesMutex);
    messages.push_back(nickname + ": " + input);

    
}

void ClientIRC::HandleIRCMessage(const std::string& ircMessage)
{
    // PONG
    if (ircMessage.rfind("PING", 0) == 0)
    {
        std::string response = "PONG" + ircMessage.substr(4) + "\r\n";
        client->Send(response.c_str(), (int)response.size());
        return;
    }

    //ERR_NICKNAMEINUSE
    if (ircMessage.find(" 433 ") != std::string::npos)
    {
        std::string msg = ExtractMessage(ircMessage);

        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[ERROR] " + msg);

        return;
    }
    //ERR_ALREADYREGISTERED
    if (ircMessage.find(" 462 ") != std::string::npos)
    {
        std::string msg = ExtractMessage(ircMessage);

        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[ERROR]  " + msg);

        return;
    }
    //ERR_NEEDMOREPARAMS
    if (ircMessage.find(" 461 ") != std::string::npos)
    {
        std::istringstream iss(ircMessage);

        std::string prefix, code, myNick, command;
        iss >> prefix >> code >> myNick >> command;

        std::string msg = ExtractMessage(ircMessage);

        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[ERROR] Not enough parameters for " + command + ": " + msg);

        return;
    }
    //ERR_NONICKNAMEGIVEN
    if (ircMessage.find(" 431 ") != std::string::npos)
    {
        std::string msg = ExtractMessage(ircMessage);

        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[ERROR] " + msg);

        return;
    }
    //ERR_UNKNOWNCOMMAND
    if (ircMessage.find(" 421 ") != std::string::npos)
    {
        std::istringstream iss(ircMessage);

        std::string prefix, code, myNick, command;
        iss >> prefix >> code >> myNick >> command;

        std::string errorMsg = ExtractMessage(ircMessage);

        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[ERROR] Unknown command (" + command + "): " + errorMsg);

        return;
    }
    // ERR_NOTREGISTERED
    if (ircMessage.find(" 451 ") != std::string::npos)
    {
        std::string msg = ExtractMessage(ircMessage);

        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[ERROR] " + msg);

        return;
    }

    // ERR_NOTONCHANNEL
    if (ircMessage.find(" 442 ") != std::string::npos)
    {
        std::string msg = ExtractMessage(ircMessage);

        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[ERROR] " + msg);

        return;
    }

    // ERR_NOSUCHCHANNEL
    if (ircMessage.find(" 403 ") != std::string::npos)
    {
        std::string msg = ExtractMessage(ircMessage);

        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[ERROR] " + msg);

        return;
    }

    // ERR_PASSWDMISMATCH
    if (ircMessage.find(" 464 ") != std::string::npos)
    {
        std::string msg = ExtractMessage(ircMessage);

        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[ERROR] Password incorrect: " + msg);

        return;
    }

    // RPL_NOTOPIC
    if (ircMessage.find(" 331 ") != std::string::npos)
    {
        std::string msg = ExtractMessage(ircMessage);

        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[TOPIC] " + msg);

        return;
    }

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
        std::string channel;

        if (pos != std::string::npos)
        {
            channel = ircMessage.substr(pos + 5);

            if (!channel.empty() && channel[0] == ':')
            {
                channel = channel.substr(1);
            }

            size_t end = channel.find(' ');

            if (end != std::string::npos)
            {
                channel = channel.substr(0, end);
            }
        }

        if (nick == nickname)
        {
            currentChannel = channel;
        }

        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[INFO] " + nick + " joined " + channel);

        return;
    }
    
    // PART
    if (ircMessage.find(" PART ") != std::string::npos)
    {
        std::string nick = ExtractNickname(ircMessage);

        size_t pos = ircMessage.find("PART ");
        std::string channel;

        if (pos != std::string::npos)
        {
            channel = ircMessage.substr(pos + 5);

            size_t end = channel.find(' ');
            if (end != std::string::npos)
            {
                channel = channel.substr(0, end);
            }
        }

        // Si c'est toi qui quitte reset currentChannel
        if (nick == nickname)
        {
            currentChannel = "";
        }

        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[INFO] " + nick + " left " + channel);

        return;
    }

    // NOTICE
    if (ircMessage.find(" NOTICE ") != std::string::npos)
    {
        std::string nick = ExtractNickname(ircMessage);
        std::string text = ExtractMessage(ircMessage);

        std::lock_guard<std::mutex> lock(messagesMutex);

        // Si c'est le serveur (pas de nick)
        if (nick == "Unknown")
        {
            messages.push_back("[NOTICE] " + text);
        }
        else
        {
            messages.push_back("[NOTICE] " + nick + ": " + text);
        }

        return;
    }

    //TOPIC
    if (ircMessage.find(" TOPIC ") != std::string::npos)
    {
        std::string nick = ExtractNickname(ircMessage);
        std::string topic = ExtractMessage(ircMessage);

        size_t pos = ircMessage.find("TOPIC ");
        std::string channel;

        if (pos != std::string::npos)
        {
            channel = ircMessage.substr(pos + 6);

            size_t end = channel.find(' ');
            if (end != std::string::npos)
            {
                channel = channel.substr(0, end);
            }
        }

        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[TOPIC] " + channel + " changed by " + nick + ": " + topic);

        return;
    }
    if (ircMessage.find(" 332 ") != std::string::npos)
    {
        std::string topic = ExtractMessage(ircMessage);

        size_t pos = ircMessage.find(" 332 ");
        std::string channel;

        if (pos != std::string::npos)
        {
            size_t start = pos + 5;
            size_t next = ircMessage.find(' ', start);

            if (next != std::string::npos)
            {
                channel = ircMessage.substr(start, next - start);
            }
        }

        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[TOPIC] " + channel + ": " + topic);

        return;
    }

    //NAMESS
    if (ircMessage.find(" 353 ") != std::string::npos)
    {
        std::string users = ExtractMessage(ircMessage);

        size_t pos = ircMessage.find(" 353 ");
        std::string channel;

        if (pos != std::string::npos)
        {
            size_t start = ircMessage.find('#', pos);
            size_t end = ircMessage.find(' ', start);

            if (start != std::string::npos && end != std::string::npos)
            {
                channel = ircMessage.substr(start, end - start);
            }
        }

        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[NAMES] " + channel + ": " + users);

        return;
    }
       
    //LISTE
    if (ircMessage.find(" 322 ") != std::string::npos)
    {
        size_t pos = ircMessage.find(" 322 ");

        std::string channel;
        std::string userCount;
        std::string topic;

        if (pos != std::string::npos)
        {
            // Trouver le channel
            size_t chanStart = ircMessage.find('#', pos);
            size_t chanEnd = ircMessage.find(' ', chanStart);

            if (chanStart != std::string::npos && chanEnd != std::string::npos)
            {
                channel = ircMessage.substr(chanStart, chanEnd - chanStart);

                // Nombre d'utilisateurs
                size_t countStart = chanEnd + 1;
                size_t countEnd = ircMessage.find(' ', countStart);

                if (countEnd != std::string::npos)
                {
                    userCount = ircMessage.substr(countStart, countEnd - countStart);
                }
            }

            // Topic
            topic = ExtractMessage(ircMessage);
        }

        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[LIST] " + channel + " (" + userCount + " users) - " + topic);

        return;
    }
    if (ircMessage.find(" 323 ") != std::string::npos)
    {
        std::string msg = ExtractMessage(ircMessage);

        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[LIST] " + msg);

        return;
    }

    //AWAY
    if (ircMessage.find(" 305 ") != std::string::npos)
    {
        std::string msg = ExtractMessage(ircMessage);

        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[AWAY] " + msg);

        return;
    }
    if (ircMessage.find(" 306 ") != std::string::npos)
    {
        std::string msg = ExtractMessage(ircMessage);

        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[AWAY] " + msg);

        return;
    }

    //WHO
    if (ircMessage.find(" 352 ") != std::string::npos)
    {
        std::istringstream iss(ircMessage);

        std::string prefix, code, myNick;
        std::string channel, username, host, server, nick, status, hopcount;

        if (!(iss >> prefix >> code >> myNick >> channel >> username >> host >> server >> nick >> status >> hopcount))
            return;

        std::string realname;
        std::getline(iss, realname);

        size_t colon = realname.find(':');
        if (colon != std::string::npos)
        {
            realname = realname.substr(colon + 1);
        }
        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[WHO] " + nick + " (" + username + "@" + host + ") - " + realname);

        return;
    }
    if (ircMessage.find(" 315 ") != std::string::npos)
    {
        std::string msg = ExtractMessage(ircMessage);

        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[WHO] " + msg);

        return;
    }
    
    //WHOIS
    if (ircMessage.find(" 311 ") != std::string::npos)
    {
        std::istringstream iss(ircMessage);

        std::string prefix, code, myNick;
        std::string nick, user, host, star;

        iss >> prefix >> code >> myNick >> nick >> user >> host >> star;

        std::string realname;
        std::getline(iss, realname);

        size_t colon = realname.find(':');
        if (colon != std::string::npos)
        {
            realname = realname.substr(colon + 1);
        }

        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[WHOIS] " + nick + " (" + user + "@" + host + ") - " + realname);
        return;
    }
    if (ircMessage.find(" 319 ") != std::string::npos)
    {
        std::string nick;
        std::istringstream iss(ircMessage);

        std::string prefix, code, myNick;
        iss >> prefix >> code >> myNick >> nick;

        std::string channels = ExtractMessage(ircMessage);

        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[WHOIS] " + nick + " is in: " + channels);

        return;
    }
    if (ircMessage.find(" 312 ") != std::string::npos)
    {
        std::istringstream iss(ircMessage);

        std::string prefix, code, myNick;
        std::string nick, server;

        iss >> prefix >> code >> myNick >> nick >> server;

        std::string info = ExtractMessage(ircMessage);

        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[WHOIS] " + nick + " server: " + server + " (" + info + ")");

        return;
    }
    if (ircMessage.find(" 318 ") != std::string::npos)
    {
        std::string msg = ExtractMessage(ircMessage);

        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back("[WHOIS] " + msg);

        return;
    }

    //PRIVMSG
    if (ircMessage.find(" PRIVMSG ") != std::string::npos)
    {
        std::string nick = ExtractNickname(ircMessage);
        std::string text = ExtractMessage(ircMessage);
    
        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push_back(nick + ": " + text);
    }

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