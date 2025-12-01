# Core library
add_library(jobtracker_core
	core/application.cpp
	core/job_tracker.cpp
	util/string_utils.cpp
	util/date_time.cpp
)

target_include_directories(jobtracker_core
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
)

# SQLite storage layer
add_subdirectory(storage)

# Import layer
add_library(jobtracker_import
	import/import_service.cpp
	import/csv_import_source.cpp
)

target_link_libraries(jobtracker_import
	PUBLIC
		jobtracker_core
)

target_include_directories(jobtracker_import
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
)

# CLI library (argument parsing)
add_library(jobtracker_cli_lib
	cli/command_line.cpp
)

target_link_libraries(jobtracker_cli_lib
	PUBLIC
		jobtracker_core
		jobtracker_storage_sqlite
		jobtracker_import
)

target_include_directories(jobtracker_cli_lib
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
)

# CLI executable (just main() + library)
add_executable(jobtracker_cli
	cli/main.cpp
)

target_link_libraries(jobtracker_cli
	PRIVATE
		jobtracker_cli_lib
)
