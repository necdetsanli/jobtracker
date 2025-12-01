#pragma once

#include <sqlite3.h>
#include "storage/application_repository.h"
#include "storage/sqlite_database.h"

/// SQLite-based implementation of IApplicationRepository.
///
/// This repository owns a SQLite database connection and ensures that the
/// required schema exists.
class SqliteApplicationRepository : public IApplicationRepository
{
public:
	/// Opens (or creates) a SQLite database at the given path and ensures the schema exists.
	///
	/// \param database_path Path to the SQLite database file. Use ":memory:" for tests.
	explicit SqliteApplicationRepository(const std::string &database_path);

	Application insert(const Application &application) override;
	bool update(const Application &application) override;
	bool remove(int id) override;
	std::vector<Application> find_all() override;
	std::optional<Application> find_by_id(int id) override;
	std::vector<Application> find_by_status(const std::string &status) override;
	Statistics compute_statistics() override;

private:
	SqliteDatabase database_;

	void ensure_schema();

	Application map_row_to_application(sqlite3_stmt *stmt) const;
};
