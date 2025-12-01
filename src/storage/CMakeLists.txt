find_package(SQLite3 REQUIRED)

add_library(jobtracker_storage_sqlite
	sqlite_database.cpp
	sqlite_application_repository.cpp
)

target_link_libraries(jobtracker_storage_sqlite
	PUBLIC
		jobtracker_core
		SQLite::SQLite3
)

target_include_directories(jobtracker_storage_sqlite
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
)
