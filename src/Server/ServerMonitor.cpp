#include "ServerMonitor.h"
#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"
#include "BaseClient.h"
#include "Server.h"
#include "ServerClient.h"

//Il y'avais pas vraiment de consignes pour "Interface conviviale" mais je suis venu avec ca, une interface minimaliste mais efficace qui gere les trucs importants.
//J'avais pensé a setup le observer pattern entre le server et l'interface mais je l'ai pas fait. C'etais juste une idee, je voulait garder ca simple.
void ServerMonitor::Display() {
    InitWindow(800, 800, "Client interface");
    SetTargetFPS(20); //Je veut pas trop de fetching inutile.
    rlImGuiSetup(true);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        rlImGuiBegin();


        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(300, 600));
        ImGui::Begin("Clients informations", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        auto clients = m_serverToDisplay.GetClientRegistry().GetClients();

        ImGui::Text("Number of clients: %d", m_serverToDisplay.GetClientCount());

        if (ImGui::BeginTable("Clients", 4)) {
            ImGui::TableSetupColumn("Nick");
            ImGui::TableSetupColumn("Username");
            ImGui::TableSetupColumn("Away");
            ImGui::TableSetupColumn("Oper");
            ImGui::TableHeadersRow();

            for (BaseClient* client : clients) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", client->GetNick().c_str());
                ImGui::TableSetColumnIndex(1); ImGui::Text("%s", client->GetUsername().c_str());
                ImGui::TableSetColumnIndex(2); ImGui::Text("%s", client->GetAwayMessage().empty() ? "No" : "Yes");
                ImGui::TableSetColumnIndex(3); ImGui::Text("%s", client->IsOper() ? "Yes" : "No");
            }
            ImGui::EndTable();
        }

        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(0, 600));
        ImGui::SetNextWindowSize(ImVec2(800, 200));
        ImGui::Begin("Notifs", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
            for (const std::string& notif : m_notifications) {
                ImGui::Text("%s", notif.c_str());
            }
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(300, 0));
        ImGui::SetNextWindowSize(ImVec2(500, 600));
        ImGui::Begin("Server controls", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        int maxClients = m_serverToDisplay.GetMaxClientCount();
        if (ImGui::InputInt("Max clients", &maxClients)) {
            if (maxClients < 0) maxClients = 0;
            m_serverToDisplay.SetMaxNumberOfClients(maxClients);
            m_notifications.push_back("Changed max clients to: " + std::to_string(maxClients) + ". Already existing clients can stay tho.");
        }
       static char msg[300] = "";
        ImGui::InputText("Broadcast msg", msg, 300);
        if (ImGui::Button("Send")) {
            for (BaseClient* client : m_serverToDisplay.GetClientRegistry().GetClients()) {
                client->Send("Message from server: " + std::string(msg) + "\r\n");
                m_notifications.push_back("Sent message to: " + client->GetNick());
            }
            msg[0] = '\0';
        }

        static char kickNick[50] = "";
        ImGui::InputText("Nick to kick", kickNick, 50);
        if (ImGui::Button("Kick")) {
            BaseClient* client = m_serverToDisplay.GetClientRegistry().FindClientByNick(kickNick);
            //De toute facon, je met pas de mock clients dans le server. sinon ca aurai pu etre un probleme
            ServerClient* serverClient = dynamic_cast<ServerClient*>(client);
            if (serverClient != nullptr) {
                serverClient->Send("ERROR :You have been kicked by the server\r\n");
                for (Channel* channel : client->GetChannels()) {
                    channel->NotifyMembers(":" + client->GetNick() + " QUIT :Kicked by server\r\n", client);
                    channel->RemoveMember(client);
                }
                m_serverToDisplay.GetClientRegistry().Remove(client);
                serverClient->Disconnect();
                m_notifications.push_back("Kicked: " + std::string(kickNick));
                kickNick[0] = '\0';
            }
        }

        ImGui::End();

        rlImGuiEnd();
        EndDrawing();
    }

}