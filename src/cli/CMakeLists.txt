# src/cli/CMakeLists.txt
#
# Command-line interface: argument parsing and the main executable.

# Library that contains reusable CLI logic (e.g., parse_arguments).
add_library(jobtracker_cli_lib
    command_line.h
    command_line.cpp
)

target_include_directories(jobtracker_cli_lib
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)

# CLI depends on core domain logic, storage and import layers.
target_link_libraries(jobtracker_cli_lib
    PUBLIC
        jobtracker_core
        jobtracker_storage_sqlite
        jobtracker_import
)

# The actual CLI executable (entry point in main.cpp).
add_executable(jobtracker_cli
    main.cpp
)

target_link_libraries(jobtracker_cli
    PRIVATE
        jobtracker_cli_lib
)
