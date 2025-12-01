# src/CMakeLists.txt
#
# Top-level source tree orchestration. Each module has its own CMakeLists.txt.

add_subdirectory(core)
add_subdirectory(storage)
add_subdirectory(import)
add_subdirectory(cli)
