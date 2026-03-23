#include "entrypoint.h"
#include "ClientSocket.h"
#include "Winsock2Init.h"
#include <string>
#include <iostream>
#include <vector>
#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"

std::string ExtractNickname(const std::string& ircMessage)
{
    if (ircMessage.size() > 0 && ircMessage[0] == ':')
    {
        size_t end = ircMessage.find('!');
        if (end != std::string::npos)
        {
            return ircMessage.substr(1, end - 1);
        }
    }
    return "Unknown";
}

std::string ExtractMessage(const std::string& ircMessage)
{
    size_t pos = ircMessage.find(" :");
    if (pos != std::string::npos)
    {
        return ircMessage.substr(pos + 2);
    }
    return ircMessage;
}

void client_start()
{
    InitWinsock2();

    ClientSocket* client = new ClientSocket(6667);
    client->Connect();

    std::string nick = "NICK Guest\r\nUSER Guest 0 * :Guest\r\n";
    client->Send(nick.c_str(), (int)nick.size());

    // std::string user = "USER Guest 0 * :Guest\r\n";
    // client->Send(user.c_str(), (int)user.size());

    std::string join = "JOIN #chat\r\n";
    client->Send(join.c_str(), (int)join.size());

    InitWindow(800, 800, "Client interface");
    SetTargetFPS(60);
    rlImGuiSetup(true);

    char inputBuffer[512] = {};
    char recvBuffer[512] = {};

    std::vector<std::string> messages;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        rlImGuiBegin();

        ImGui::Begin("Client Chat");

        // zone d'affichage des messages
        ImGui::BeginChild("ChatHistory", ImVec2(0, 500), true);

        for (const std::string& msg : messages)
        {
            ImGui::TextWrapped("%s", msg.c_str());
        }

        ImGui::EndChild();

        // champ de texte
        ImGui::InputText("Message", inputBuffer, sizeof(inputBuffer));

        if (ImGui::Button("Send"))
        {
            if (strlen(inputBuffer) > 0)
            {
                std::string msg = "PRIVMSG #chat :" + std::string(inputBuffer) + "\r\n";

                client->Send(msg.c_str(), (int)msg.size());

                messages.push_back("You: " + std::string(inputBuffer));

                if (client->WaitForResponse(recvBuffer, sizeof(recvBuffer)))
                {
                    std::string ircMessage = recvBuffer;
                    if (ircMessage.rfind("PING", 0) == 0)
                    {
                        std::string response = "PONG" + ircMessage.substr(4) + "\r\n";
                        client->Send(response.c_str(), (int)response.size());
                    }
                    else if (ircMessage.find(" 372 ") != std::string::npos || 
                        ircMessage.find(" 375 ") != std::string::npos || 
                        ircMessage.find(" 376 ") != std::string::npos)
                    {
                        std::string text = ExtractMessage(ircMessage);
                        messages.push_back("[MOTD] " + text);
                    }
                    else if (ircMessage.find(" 251 ") != std::string::npos ||
                        ircMessage.find(" 252 ") != std::string::npos ||
                        ircMessage.find(" 253 ") != std::string::npos ||
                        ircMessage.find(" 254 ") != std::string::npos ||
                        ircMessage.find(" 255 ") != std::string::npos)
                    {
                        std::string text = ExtractMessage(ircMessage);
                        messages.push_back("[LUSERS] " + text);
                    }
                    else if (ircMessage.find(" JOIN ") != std::string::npos)
                    {
                        std::string nick = ExtractNickname(ircMessage);

                        size_t pos = ircMessage.find("JOIN ");
                        std::string channel = (pos != std::string::npos)
                        ? ircMessage.substr(pos + 5)
                        : "";

                        messages.push_back("[INFO] " + nick + " joined " + channel);
                    }
                    else
                    {
                        std::string nick = ExtractNickname(ircMessage);
                        std::string text = ExtractMessage(ircMessage);

                        messages.push_back(nick + ": " + text);
                    }
                }

                inputBuffer[0] = '\0';
                }
        }

        ImGui::End();

        rlImGuiEnd();
        EndDrawing();
    }

    delete client;
    rlImGuiShutdown();
    CloseWindow();
    DeInitWinsock2();
}
