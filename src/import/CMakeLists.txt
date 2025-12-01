# src/import/CMakeLists.txt
#
# Import layer: CSV and IMAP-based import sources and the ImportService
# that coordinates them.

# Require CURL for HTTP operations in remote CSV import source
find_package(CURL REQUIRED)

add_library(jobtracker_import
    # CSV-based import sources
    csv_import_source.h
    csv_import_source.cpp

    # Remote CSV over HTTP
    remote_csv_import_source.h
    remote_csv_import_source.cpp

    # HTTP-based import source
    http_client.h
    http_client.cpp

    # High-level import service
    import_service.h
    import_service.cpp

    # IMAP / email-related
    email_message.h
    imap_email_client.h
    imap_import_source.h
    imap_import_source.cpp

    # Import abstraction
    import_source.h
)


# Public include dir so that other targets can include "import/..."
target_include_directories(jobtracker_import
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)

# Import logic depends on the core domain model (Application, JobTracker, etc.).
target_link_libraries(jobtracker_import
	PRIVATE
		jobtracker_core
		jobtracker_storage_sqlite
		CURL::libcurl
)
