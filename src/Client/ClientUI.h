#pragma once
#include "clientIRC.h"
#include "ClientSocket.h"
#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"

class ClientUI
{
private:
    ClientIRC* irc;
    ClientSocket* client;

    char inputBuffer[512];

public:
    ClientUI(ClientIRC* irc, ClientSocket* client);

    void Init();
    void Update();
    void Draw();
    bool ShouldClose();
    void Shutdown();
};