#include "entrypoint.h"
#include "ClientSocket.h"
#include <string>
#include <iostream>
#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"

void client_start()
{
    
    ClientSocket* client = new ClientSocket(6667, (char*)"127.0.0.1");

    client->Connect();

    // std::string msg = "Hello server\r\n";
    // client->Send(msg.c_str(), (int)msg.size());

    // char buffer[512];
    // client->WaitForResponse(buffer, sizeof(buffer));

    // std::cout << "Server says: " << buffer << std::endl;

    // delete client;
    //Vieux code pour référence si jamais.

    InitWindow(800, 800, "Client interface");
    SetTargetFPS(60);
    rlImGuiSetup(true);

    char inputBuffer[512] = {};
    char recvBuffer[512] = {};

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        rlImGuiBegin();
        ImGui::Begin("Client");

        ImGui::InputText("Message", inputBuffer, sizeof(inputBuffer));
        
        if (ImGui::Button("Send")) {
            if (strlen(inputBuffer) > 0) {
                std::string msg = std::string(inputBuffer) + "\r\n";
                client->Send(msg.c_str(), (int)msg.size());
                if (client->WaitForResponse(recvBuffer, sizeof(recvBuffer))) {
                    std::cout << "Server: " << recvBuffer << std::endl;
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
}
