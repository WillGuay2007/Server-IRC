#include "entrypoint.h"
#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"

void raylib_start(void)
{
    InitWindow(800, 600, "Raylib + ImGui");
    SetTargetFPS(60);

    rlImGuiSetup(true);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        DrawText("Hello", 20, 20, 20, RAYWHITE);

        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
}
