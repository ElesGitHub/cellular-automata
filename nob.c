// Re-define rebuild command to include header files
#if defined(_WIN32)
#  if defined(__clang__)
#    if defined(__cplusplus)
#      define NOB_REBUILD_URSELF(binary_path, source_path) "clang", "-x", "c++", "-Iincludes", "-o", binary_path, source_path
#    else
#      define NOB_REBUILD_URSELF(binary_path, source_path) "clang", "-x", "c", "-Iincludes", "-o", binary_path, source_path
#    endif
#  elif defined(__GNUC__)
#    if defined(__cplusplus)
#      define NOB_REBUILD_URSELF(binary_path, source_path) "gcc", "-x", "c++", "-Iincludes", "-o", binary_path, source_path
#    else
#      define NOB_REBUILD_URSELF(binary_path, source_path) "gcc", "-x", "c", "-Iincludes", "-o", binary_path, source_path
#    endif
#  elif defined(_MSC_VER)
#     define NOB_REBUILD_URSELF(binary_path, source_path) "cl.exe", nob_temp_sprintf("/Fe:%s", (binary_path)), source_path
#  elif defined(__TINYC__)
#     define NOB_REBUILD_URSELF(binary_path, source_path) "tcc", "-Iincludes", "-o", binary_path, source_path
#  endif
#else
#  if defined(__cplusplus)
#    define NOB_REBUILD_URSELF(binary_path, source_path) "cc", "-x", "c++", "-Iincludes", "-o", binary_path, source_path
#  else
#    define NOB_REBUILD_URSELF(binary_path, source_path) "cc", "-x", "c", "-Iincludes", "-o", binary_path, source_path
#  endif
#endif

#define NOB_IMPLEMENTATION
#include <nob.h>

#define CC "gcc"
#define SRC_PATH "main.c"

#if defined(_WIN32)
#   define OUT_PATH "main.exe"
#   define SYS_LIBS "-lgdi32", "-lwinmm"
#else
#   define OUT_PATH "main.out"
#endif // defined(_WIN32)

int main(int argc, char** argv) {
    GO_REBUILD_URSELF(argc, argv);

    Cmd cmd = {0};

    
    cmd_append(&cmd, CC);
    cmd_append(&cmd, "-o", OUT_PATH, SRC_PATH);
    cmd_append(&cmd, "-Iincludes");
    cmd_append(&cmd, "-Llibs", "-lraylib");
    cmd_append(&cmd, SYS_LIBS);
    cmd_run(&cmd);

    return 0;
}