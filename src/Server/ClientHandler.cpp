#include "ClientHandler.h"

#include "ServerClient.h"
#include "ClientRegistry.h"
#include "Channel.h"
#include "CommandDispatcher.h"
#include "IrcMessage.h"

#include "JoinHandler.h"
#include "NickHandler.h"
#include "UserHandler.h"
#include "PingHandler.h"
#include "MOTDHandler.h"

ClientHandler::~ClientHandler() {
    delete m_commandDispatcher;
}

void ClientHandler::Handle() {
    std::cout << "Client connected\n";

    char buffer[500];

    while (true)
    {
        if (!m_client.Receive(buffer, sizeof(buffer)))
        {
            break; //Déconnecter le client si ca fail.
        }

        std::string clientResponse(buffer);

        size_t pos;
        while ((pos = clientResponse.find("\r\n")) != std::string::npos)
        {
            std::string line = clientResponse.substr(0, pos);
            clientResponse.erase(0, pos + 2);

            IrcMessage msg = IrcMessage::Parse(line);

            m_commandDispatcher->Dispatch(msg);
        }
    }

    std::cout << "Client disconnected\n";
}

void ClientHandler::InitCommandDispatcher() {
    std::unordered_map<std::string, Handler*> handlersMap {
        {"NICK", new NickHandler(m_client, m_registry)},
        {"USER", new UserHandler(m_client)},
        {"MOTD", new MOTDHandler()},
        {"PING", new PingHandler()},
        {"JOIN", new JoinHandler(m_client, m_channels)},
    };

    m_commandDispatcher = new CommandDispatcher(handlersMap, &m_client);
}