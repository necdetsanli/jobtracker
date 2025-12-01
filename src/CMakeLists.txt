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

add_executable(jobtracker_cli
	cli/main.cpp
)

target_link_libraries(jobtracker_cli
	PRIVATE
		jobtracker_core
		jobtracker_storage_sqlite
)
