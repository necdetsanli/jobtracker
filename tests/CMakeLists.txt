find_package(Catch2 3 REQUIRED)

set(TEST_SOURCES
	test_skeleton.cpp
	util/test_string_utils.cpp
	util/test_date_time.cpp
	core/test_job_tracker.cpp
)

add_executable(jobtracker_tests ${TEST_SOURCES})

target_link_libraries(jobtracker_tests
	PRIVATE
		jobtracker_core
		Catch2::Catch2WithMain
)

# Allow tests to include headers from the tests/ tree (e.g. core/fake_application_repository.h)
target_include_directories(jobtracker_tests
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
)

include(CTest)
add_test(NAME jobtracker_tests COMMAND jobtracker_tests)
