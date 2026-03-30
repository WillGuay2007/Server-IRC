#include "entrypoint.h"
#include "ClientSocket.h"
#include "clientIRC.h"
#include "Winsock2Init.h"
#include <string>
#include <iostream>
#include <vector>
#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"

void client_start()
{
    InitWinsock2();

    ClientSocket client(6667);
    client.Connect();

    std::string nickname = "Guest";

    std::string login = "NICK " + nickname + "\r\nUSER " + nickname + "0 * :" + nickname + "\r\n";
    client.Send(login.c_str(), (int)login.size());

    std::string join = "JOIN #chat\r\n";
    client.Send(join.c_str(), (int)join.size());

    InitWindow(800, 800, "Client interface");
    SetTargetFPS(60);
    rlImGuiSetup(true);

    char inputBuffer[512] = {};
    char recvBuffer[512] = {};

    std::vector<std::string> messages;

    while (!WindowShouldClose())
    {

        if (client.WaitForResponse(recvBuffer, sizeof(recvBuffer)))
        {
            std::string ircMessage = recvBuffer;
            HandleIRCMessage(ircMessage, &client, messages, nickname);
        }

        BeginDrawing();
        ClearBackground(DARKGRAY);

        rlImGuiBegin();

        ImGui::Begin("Client Chat");

        ImGui::BeginChild("ChatHistory", ImVec2(0, 500), true);

        for (const std::string& msg : messages)
        {
            ImGui::TextWrapped("%s", msg.c_str());
        }

        ImGui::EndChild();

        if (ImGui::InputText("Message", inputBuffer, sizeof(inputBuffer),
            ImGuiInputTextFlags_EnterReturnsTrue))
        {
            if (strlen(inputBuffer) > 0)
            {
                std::string msg = "PRIVMSG #chat :" + std::string(inputBuffer) + "\r\n";

                client.Send(msg.c_str(), (int)msg.size());

                messages.push_back("You: " + std::string(inputBuffer));

                inputBuffer[0] = '\0';
            }
        }

        if (ImGui::Button("Send"))
        {
            if (strlen(inputBuffer) > 0)
            {
                std::string msg = "PRIVMSG #chat :" + std::string(inputBuffer) + "\r\n";

                client.Send(msg.c_str(), (int)msg.size());

                messages.push_back("You: " + std::string(inputBuffer));

                inputBuffer[0] = '\0';
            }
        }

        ImGui::End();

        rlImGuiEnd();
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    DeInitWinsock2();
}