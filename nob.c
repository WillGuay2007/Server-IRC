#define nob_cc(cmd) nob_cmd_append(cmd, "./Tools/w64devkit/bin/c++.exe")

#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

#include "noblib_rlimgui.c"

void addSharedSources(Cmd *cmd) {
    cmd_append(cmd, "-Isrc/SharedUtils");
    cmd_append(cmd, "src/SharedUtils/Socket.cpp");
    cmd_append(cmd, "src/SharedUtils/ClientSocket.cpp");
    cmd_append(cmd, "src/SharedUtils/ServerSocket.cpp");
    cmd_append(cmd, "src/SharedUtils/Winsock2Init.cpp");
}

void addSharedLibs(Cmd *cmd) {
    cmd_append(cmd, "-lws2_32");
}


void build_server(void)
{
    Cmd cmd = {0};

    nob_cc(&cmd);
    cmd_append(&cmd, "-ggdb3");

    cmd_append(&cmd, "src/Server/main.cpp");
    cmd_append(&cmd, "src/Server/entrypoint.cpp");


    addSharedSources(&cmd);

    nob_cc_output(&cmd, "./Deployment/server.exe");

    addSharedLibs(&cmd);

    if (!cmd_run_sync_and_reset(&cmd)) exit(1);
}

void build_client(File_Paths* o_files)
{
    Cmd cmd = {0};

    nob_cc(&cmd);
    cmd_append(&cmd, "-ggdb3");

    cmd_append(&cmd, "src/Client/main.cpp");
    cmd_append(&cmd, "src/Client/entrypoint.cpp");


    cmd_append(&cmd, RAYLIB_INCLUDES);
    cmd_append(&cmd, RLIMGUI_INCLUDES);

    for (int i = 0; i < o_files->count; ++i) {
        cmd_append(&cmd, o_files->items[i]);
    }

    addSharedSources(&cmd);

    nob_cc_output(&cmd, "./Deployment/client.exe");

    addSharedLibs(&cmd);

    cmd_append(&cmd, RAYLIB_LFLAGS);
    cmd_append(&cmd, RLIMGUI_LFLAGS);

    if (!cmd_run_sync_and_reset(&cmd)) exit(1);
}

int main(int argc, char** argv)
{
    NOB_GO_REBUILD_URSELF_PLUS(argc, argv, RAYLIB_FILE);

    if (!mkdir_if_not_exists("Deployment")) return 1;
    if (!mkdir_if_not_exists("build")) return 1;
    if (!mkdir_if_not_exists("build/Libraries")) return 1;

    File_Paths o_files = {0};
    if (!build_raylib(&o_files)) return 1;
    if (!build_rlImGui(&o_files)) return 1;

    if (argc < 2) {
        printf("Usage: nob client | server\n");
        return 1;
    }

    if (strcmp(argv[1], "client") == 0) {
        build_client(&o_files);
    }
    else if (strcmp(argv[1], "server") == 0) {
        build_server();
    }
    else {
        printf("Unknown target: %s\n", argv[1]);
        return 1;
    }

    return 0;
}
