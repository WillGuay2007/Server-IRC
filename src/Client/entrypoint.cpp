#include "entrypoint.h"
#include "ClientSocket.h"
#include "clientIRC.h"
#include <iostream>
#include <thread>
#include <atomic>
#include "ClientUI.h"

void NetworkLoop(ClientSocket* client, ClientIRC* irc, std::atomic<bool>& running)
{
    char buffer[512];
    std::string pending;
    
    while (running)
    {
        memset(buffer, 0, sizeof(buffer));
        if (client->WaitForResponse(buffer, sizeof(buffer)))
        {
            pending += buffer;

        size_t pos;
        while ((pos = pending.find("\r\n")) != std::string::npos)
        {
            std::string line = pending.substr(0, pos);
            pending.erase(0, pos + 2);

            irc->HandleIRCMessage(line);
        }
        }
    }
}

void client_start(const std::string& address)
{
    std::atomic<bool> running(true);

    std::string ip = "127.0.0.1";
    int port = 6667;

    std::string parsedAddress = address;

    if (parsedAddress.rfind("irc://", 0) == 0)
    {
        parsedAddress = parsedAddress.substr(6);
    }

    size_t colon = parsedAddress.find(':');

    if (colon != std::string::npos)
    {
        ip = parsedAddress.substr(0, colon);
        port = std::stoi(parsedAddress.substr(colon + 1));
    }
    else
    {
        ip = parsedAddress;
    }

    ClientSocket client(port, (char*)ip.c_str());
    client.Connect();

    ClientIRC irc(&client);
    
    std::string nickname = "Guest";
    std::string login = "NICK " + nickname + "\r\nUSER " + nickname + " 0 * :" + nickname + "\r\n";
    client.Send(login.c_str(), (int)login.size());
    
    client.Send("JOIN #chat\r\n", 12);
    
    std::thread netThread(NetworkLoop, &client, &irc, std::ref(running));

    ClientUI ui(&irc, &client);
    ui.Init();

    while (!ui.ShouldClose())
    {
        ui.Update();
        ui.Draw();
    }

    running = false;
    netThread.join();

    ui.Shutdown();
}