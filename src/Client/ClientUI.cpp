#include "ClientUI.h"
#include <cstring>

ClientUI::ClientUI(ClientIRC* irc, ClientSocket* client)
{
    this->irc = irc;
    this->client = client;
    memset(inputBuffer, 0, sizeof(inputBuffer));
}

void ClientUI::Init()
{
    InitWindow(800, 800, "Client interface");
    SetTargetFPS(60);
    rlImGuiSetup(true);
}

bool ClientUI::ShouldClose()
{
    return WindowShouldClose();
}

void ClientUI::Update()
{

}

void ClientUI::Draw()
{
    BeginDrawing();
    ClearBackground(DARKGRAY);

    rlImGuiBegin();

    ImGui::Begin("Client Chat");

    ImGui::BeginChild("ChatHistory", ImVec2(0, 500), true);

    auto messages = irc->GetMessages();

    for (const std::string& msg : messages)
    {
        ImGui::TextWrapped("%s", msg.c_str());
    }

    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
    {
    ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();

    bool sendMessage = false;

    if (ImGui::InputText("Message", inputBuffer, sizeof(inputBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
    {
        sendMessage = true;
    }

    ImGui::SameLine();

    if (ImGui::Button("Send"))
    {
        sendMessage = true;
    }

    if (sendMessage && strlen(inputBuffer) > 0)
    {
        irc->SendUserInput(inputBuffer);
        inputBuffer[0] = '\0';
    }

    ImGui::End();

    rlImGuiEnd();
    EndDrawing();
}

void ClientUI::Shutdown()
{
    rlImGuiShutdown();
    CloseWindow();
}