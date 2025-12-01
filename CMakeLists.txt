cmake_minimum_required(VERSION 3.16)

project(jobtracker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Toolchain-specific compiler options
if(MSVC)
    # MSVC: enable high warning level and strict standard conformance
    add_compile_options(/W4 /permissive- /EHsc)
else()
    # GCC/Clang: enable common warning flags
    add_compile_options(-Wall -Wextra -Wpedantic)

    # Sanitizers only on non-MSVC compilers in Debug builds
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    endif()
endif()

# ---- Dependencies ----
find_package(SQLite3 REQUIRED)
find_package(Catch2 3 REQUIRED) # Only if you prefer to find it here; otherwise in tests/CMakeLists.txt

# ---- Subdirectories ----
add_subdirectory(src)
add_subdirectory(tests)
