# src/core/CMakeLists.txt
#
# Core domain model: Application, JobTracker and related logic.

add_library(jobtracker_core
    # Core headers
    application.h
    application.cpp
    job_tracker.h
    job_tracker.cpp

    # Util headers/sources (located under src/util)
    ../util/string_utils.h
    ../util/string_utils.cpp
    ../util/date_time.h
    ../util/date_time.cpp
)

# Expose src/ as a public include root so that headers can be included as
# "core/..." and "util/..." from other targets.
target_include_directories(jobtracker_core
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)
