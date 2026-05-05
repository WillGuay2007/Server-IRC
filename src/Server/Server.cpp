#include "Server.h"
#include <iostream>
#include <string.h>
#include "ServerSocket.h"
#include "ClientSocket.h"
#include <thread>
#include "ServerClient.h"
#include <vector>
#include "ServerResponses.h"
#include "UnitTest.h"
#include "ClientHandler.h"

#include "JoinHandler.h"
#include "NickHandler.h"
#include "UserHandler.h"
#include "PingHandler.h"
#include "MOTDHandler.h"
#include "PrivMsgHandler.h"

void Server::Start() {
    ServerSocket serverSocket(6667);
    serverSocket.StartListening();

    std::cout << "Server running on port 6667...\n";

    while (true)
    {
        ClientSocket* clientSocket = serverSocket.WaitForConnection();
        ServerClient* client = new ServerClient(clientSocket);
        if (clientSocket == nullptr) continue;
        m_clientRegistry.Add(client);
        std::thread clientThread([client, this]() {
            ClientHandler clientHandler = ClientHandler(*client, m_commandDispatcher);
            clientHandler.Handle();
            m_clientRegistry.Remove(client);
            delete client;
        });
        clientThread.detach();
    }
}

CommandDispatcher Server::CreateCommandDispatcher() {
     std::unordered_map<std::string, Handler*> handlersMap {
        {"NICK", new NickHandler(m_clientRegistry)},
        {"USER", new UserHandler()},
        {"MOTD", new MOTDHandler()},
        {"PING", new PingHandler()},
        {"JOIN", new JoinHandler(m_channelRegistry)},
        {"PRIVMSG", new PrivMsgHandler(m_clientRegistry, m_channelRegistry)}
    };

    return CommandDispatcher(handlersMap);
}