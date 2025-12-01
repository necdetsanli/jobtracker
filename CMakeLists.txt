cmake_minimum_required(VERSION 3.16)

project(jobtracker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Warnings
add_compile_options(-Wall -Wextra -Wpedantic)

# Sanitizers (Debug'da açık)
option(ENABLE_SANITIZERS "Enable Address/Undefined sanitizers in Debug builds" ON)

if (ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT MSVC)
	add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
	add_link_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
endif()

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)
