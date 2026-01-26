#define nob_cc(cmd) nob_cmd_append(cmd, "./Tools/w64devkit/bin/cc.exe")

#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"


#include "noblib_rlimgui.c"

bool read_entire_dir_visit(Nob_Walk_Entry entry){
  String_View sv = sv_from_cstr(entry.path);
  if(sv_end_with(sv,".cpp")){
    Cmd* cmd = entry.data;
    cmd_append(cmd,strdup(entry.path));
  }
  return true;
}

int main(int argc, char** argv){
  NOB_GO_REBUILD_URSELF_PLUS(argc, argv,RAYLIB_FILE);
  // ^--- Only needed if you intend on changing the build file, use NOB_GO_REBUILD_URSELF otherwise
  if (!mkdir_if_not_exists("Deployment")){ return 1;}
  if (!mkdir_if_not_exists("build")){ return 1;}
  if (!mkdir_if_not_exists("build/Libraries")){ return 1;}
  // ^---- UPDATE THE PATH IF LIBRARIES ARE SOMEWHERE ELSE
  File_Paths o_files = {0};
  if(!build_raylib(&o_files)) return 1;
  if(!build_rlImGui(&o_files)) return 1;

  Cmd cmd = {0};

  nob_cc(&cmd);
  cmd_append(&cmd,"-ggdb3");
  nob_walk_dir("./src", read_entire_dir_visit, .data = &cmd );
  // nob_cc_inputs(&cmd, "./src/main.cpp",temp_sprintf("./src/%s.cpp",LAB_NAME));
  cmd_append(&cmd,RAYLIB_INCLUDES);
  cmd_append(&cmd,RLIMGUI_INCLUDES);
  for(int i =0; i < o_files.count;++i){
    cmd_append(&cmd,o_files.items[i]);
  }
  nob_cc_output(&cmd, "./Deployment/game.exe");
  cmd_append(&cmd,RAYLIB_LFLAGS);
  cmd_append(&cmd,RLIMGUI_LFLAGS);
  if(!cmd_run_sync_and_reset(&cmd)) return 1;
}