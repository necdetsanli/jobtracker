# src/CMakeLists.txt

# src/CMakeLists.txt

# Core library: jobtracker_core
add_library(jobtracker_core
	core/application.cpp
	core/job_tracker.cpp
	util/string_utils.cpp
	util/date_time.cpp
)

# Public include path for the core library
target_include_directories(jobtracker_core
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
)

# CLI executable
add_executable(jobtracker_cli
	cli/main.cpp
)

target_link_libraries(jobtracker_cli
	PRIVATE
		jobtracker_core
)


# Later: Qt GUI executable (placeholder for future)
#
# find_package(Qt6 COMPONENTS Core Widgets REQUIRED)
#
# add_executable(jobtracker_gui
#     gui/main_qt.cpp
#     gui/JobTrackerWindow.cpp
#     gui/ApplicationModel.cpp
# )
#
# target_link_libraries(jobtracker_gui
#     PRIVATE
#         jobtracker_core
#         Qt6::Core
#         Qt6::Widgets
# )
