# src/import/CMakeLists.txt
#
# Import layer: CSV and IMAP-based import sources and the ImportService
# that coordinates them.

add_library(jobtracker_import
    # Interfaces and core import types
    import_source.h
    import_service.h
    import_service.cpp

    # CSV-based import source
    csv_import_source.h
    csv_import_source.cpp

    # IMAP-based import source
    imap_import_source.h
    imap_import_source.cpp

    # Email abstractions
    email_client.h
    email_message.h

    # Placeholder for a future real IMAP client implementation
    imap_email_client.h
    # imap_email_client.cpp  # Add here when you implement it
)

# Public include dir so that other targets can include "import/..."
target_include_directories(jobtracker_import
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)

# Import logic depends on the core domain model (Application, JobTracker, etc.).
target_link_libraries(jobtracker_import
    PUBLIC
        jobtracker_core
)
