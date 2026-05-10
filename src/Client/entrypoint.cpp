#include "entrypoint.h"
#include "ClientSocket.h"
#include "clientIRC.h"
#include <string>
#include <iostream>
#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"
#include <thread>
#include <atomic>


void NetworkLoop(ClientSocket* client, ClientIRC* irc, std::atomic<bool>& running)
{
    char buffer[512];
    
    while (running)
    {
        if (client->WaitForResponse(buffer, sizeof(buffer)))
        {
            irc->HandleIRCMessage(buffer);
        }
    }
}

void client_start()
{
    std::atomic<bool> running(true);

    ClientSocket client(6667);
    client.Connect();

    ClientIRC irc(&client);
    
    std::string nickname = "Guest";
    std::string login = "NICK " + nickname + "\r\nUSER " + nickname + " 0 * :" + nickname + "\r\n";
    client.Send(login.c_str(), (int)login.size());
    
    client.Send("JOIN #chat\r\n", 12);
    
    std::thread netThread(NetworkLoop, &client, &irc, std::ref(running));

    InitWindow(800, 800, "Client interface");
    SetTargetFPS(60);
    rlImGuiSetup(true);

    char inputBuffer[512] = {};
    char recvBuffer[512] = {};

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        rlImGuiBegin();

        ImGui::Begin("Client Chat");

        ImGui::BeginChild("ChatHistory", ImVec2(0, 500), true);
        auto messages = irc.GetMessages();
        for (const std::string& msg : irc.GetMessages())
        {
            ImGui::TextWrapped("%s", msg.c_str());
        }

        ImGui::EndChild();

        if (ImGui::InputText("Message", inputBuffer, sizeof(inputBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            if (strlen(inputBuffer) > 0)
            {
                std::string msg = "PRIVMSG #chat :" + std::string(inputBuffer) + "\r\n";

                client.Send(msg.c_str(), (int)msg.size());
                irc.HandleIRCMessage(":" + irc.GetNickname() + "! PRIVMSG #chat :" + std::string(inputBuffer));
                inputBuffer[0] = '\0';
            }
        }

        ImGui::End();

        rlImGuiEnd();
        EndDrawing();
    }
    running = false;
    netThread.join();

    rlImGuiShutdown();
    CloseWindow();
}