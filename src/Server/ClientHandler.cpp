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

void ClientHandler::Handle() {
    std::cout << "Client connected\n";

    char buffer[500];

    while (true)
    {
        if (!m_client.Receive(buffer, sizeof(buffer))) break;

        std::string clientResponse(buffer);

        size_t pos;
        while ((pos = clientResponse.find("\r\n")) != std::string::npos)
        {
            std::string line = clientResponse.substr(0, pos);

            //Pour le debug, mettre en commentaire si on utilise pas.
            std::cout << line << std::endl;

            clientResponse.erase(0, pos + 2);

            IrcMessage msg = IrcMessage::Parse(line);

            bool success = m_commandDispatcher.Dispatch(msg, m_client);
            if (!success) m_client.Send("Command " + msg.GetCommand() + " not found.\n");

        }
    }

    std::cout << "Client disconnected\n";
}