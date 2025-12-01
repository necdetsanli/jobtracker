# tests/CMakeLists.txt
#
# Test runner using Catch2 v3.

find_package(Catch2 3 REQUIRED)

set(TEST_SOURCES
	test_skeleton.cpp
	util/test_string_utils.cpp
	util/test_date_time.cpp
	cli/test_command_line.cpp
	import/test_csv_import_source.cpp
	import/test_import_service.cpp
	import/test_imap_import_source.cpp
	import/test_remote_csv_import_source.cpp
)

add_executable(jobtracker_tests
    ${TEST_SOURCES}
)

target_link_libraries(jobtracker_tests
    PRIVATE
        jobtracker_core
        jobtracker_storage_sqlite
        jobtracker_import
        jobtracker_cli_lib
        Catch2::Catch2WithMain
)

target_include_directories(jobtracker_tests
	PRIVATE
		${PROJECT_SOURCE_DIR}
)

include(CTest)
add_test(NAME jobtracker_tests COMMAND jobtracker_tests)
