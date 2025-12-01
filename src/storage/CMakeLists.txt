# src/storage/CMakeLists.txt
#
# SQLite-based persistence for Application entities.

add_library(jobtracker_storage_sqlite
    sqlite_database.h
    sqlite_database.cpp
    sqlite_application_repository.h
    sqlite_application_repository.cpp
)

target_include_directories(jobtracker_storage_sqlite
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(jobtracker_storage_sqlite
    PUBLIC
        jobtracker_core
        SQLite::SQLite3
)
