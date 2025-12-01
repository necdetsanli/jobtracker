# tests/CMakeLists.txt

find_package(Catch2 3 REQUIRED)

# Initial test sources. More files will be added as the test suite grows.
set(TEST_SOURCES
    test_skeleton.cpp
    # core/test_JobTracker.cpp
    # util/test_StringUtils.cpp
    # storage/test_SqliteRepository.cpp
)

add_executable(jobtracker_tests ${TEST_SOURCES})

target_link_libraries(jobtracker_tests
    PRIVATE
        jobtracker_core
        Catch2::Catch2WithMain
)

include(CTest)
add_test(NAME jobtracker_tests COMMAND jobtracker_tests)
