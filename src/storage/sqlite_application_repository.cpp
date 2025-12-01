/// \file
/// \brief SQLite-based implementation of IApplicationRepository.

#include "storage/sqlite_application_repository.h"

#include <sqlite3.h>

#include <stdexcept>

SqliteApplicationRepository::SqliteApplicationRepository(const std::string &database_path)
	: database_(database_path)
{
	ensure_schema();
}

void SqliteApplicationRepository::ensure_schema()
{
	const std::string sql =
		"CREATE TABLE IF NOT EXISTS applications ("
		"  id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"  company TEXT NOT NULL,"
		"  position TEXT NOT NULL,"
		"  location TEXT,"
		"  source TEXT,"
		"  status TEXT NOT NULL,"
		"  applied_date TEXT,"
		"  last_update TEXT,"
		"  notes TEXT"
		");";

	database_.execute_non_query(sql);
}

Application SqliteApplicationRepository::map_row_to_application(sqlite3_stmt *stmt) const
{
	Application app;

	app.id = sqlite3_column_int(stmt, 0);
	app.company = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
	app.position = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));

	const auto *location_text = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
	const auto *source_text = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
	const auto *status_text = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
	const auto *applied_date_text = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6));
	const auto *last_update_text = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 7));
	const auto *notes_text = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 8));

	if (location_text != nullptr)
	{
		app.location = location_text;
	}
	if (source_text != nullptr)
	{
		app.source = source_text;
	}
	if (status_text != nullptr)
	{
		app.status = status_text;
	}
	if (applied_date_text != nullptr)
	{
		app.applied_date = applied_date_text;
	}
	if (last_update_text != nullptr)
	{
		app.last_update = last_update_text;
	}
	if (notes_text != nullptr)
	{
		app.notes = notes_text;
	}

	return app;
}

Application SqliteApplicationRepository::insert(const Application &application)
{
	const char *sql =
		"INSERT INTO applications ("
		"  company, position, location, source, status, applied_date, last_update, notes"
		") VALUES (?, ?, ?, ?, ?, ?, ?, ?);";

	sqlite3 *db = database_.handle();
	sqlite3_stmt *stmt = nullptr;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
	{
		throw std::runtime_error("Failed to prepare INSERT statement");
	}

	const int rc_bind_company = sqlite3_bind_text(
		stmt,
		1,
		application.company.c_str(),
		-1,
		SQLITE_TRANSIENT
	);
	const int rc_bind_position = sqlite3_bind_text(
		stmt,
		2,
		application.position.c_str(),
		-1,
		SQLITE_TRANSIENT
	);
	const int rc_bind_location = sqlite3_bind_text(
		stmt,
		3,
		application.location.c_str(),
		-1,
		SQLITE_TRANSIENT
	);
	const int rc_bind_source = sqlite3_bind_text(
		stmt,
		4,
		application.source.c_str(),
		-1,
		SQLITE_TRANSIENT
	);
	const int rc_bind_status = sqlite3_bind_text(
		stmt,
		5,
		application.status.c_str(),
		-1,
		SQLITE_TRANSIENT
	);
	const int rc_bind_applied = sqlite3_bind_text(
		stmt,
		6,
		application.applied_date.c_str(),
		-1,
		SQLITE_TRANSIENT
	);
	const int rc_bind_update = sqlite3_bind_text(
		stmt,
		7,
		application.last_update.c_str(),
		-1,
		SQLITE_TRANSIENT
	);
	const int rc_bind_notes = sqlite3_bind_text(
		stmt,
		8,
		application.notes.c_str(),
		-1,
		SQLITE_TRANSIENT
	);

	if (rc_bind_company != SQLITE_OK ||
		rc_bind_position != SQLITE_OK ||
		rc_bind_location != SQLITE_OK ||
		rc_bind_source != SQLITE_OK ||
		rc_bind_status != SQLITE_OK ||
		rc_bind_applied != SQLITE_OK ||
		rc_bind_update != SQLITE_OK ||
		rc_bind_notes != SQLITE_OK)
	{
		sqlite3_finalize(stmt);
		throw std::runtime_error("Failed to bind INSERT parameters");
	}

	const int rc_step = sqlite3_step(stmt);
	if (rc_step != SQLITE_DONE)
	{
		sqlite3_finalize(stmt);
		throw std::runtime_error("Failed to execute INSERT statement");
	}

	sqlite3_finalize(stmt);

	Application stored = application;
	stored.id = static_cast<int>(sqlite3_last_insert_rowid(db));
	return stored;
}

bool SqliteApplicationRepository::update(const Application &application)
{
	const char *sql =
		"UPDATE applications SET "
		"  company = ?,"
		"  position = ?,"
		"  location = ?,"
		"  source = ?,"
		"  status = ?,"
		"  applied_date = ?,"
		"  last_update = ?,"
		"  notes = ?"
		"WHERE id = ?;";

	sqlite3 *db = database_.handle();
	sqlite3_stmt *stmt = nullptr;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
	{
		throw std::runtime_error("Failed to prepare UPDATE statement");
	}

	sqlite3_bind_text(stmt, 1, application.company.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, application.position.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 3, application.location.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 4, application.source.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 5, application.status.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 6, application.applied_date.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 7, application.last_update.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 8, application.notes.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 9, application.id);

	const int rc_step = sqlite3_step(stmt);
	if (rc_step != SQLITE_DONE)
	{
		sqlite3_finalize(stmt);
		throw std::runtime_error("Failed to execute UPDATE statement");
	}

	const int changes = sqlite3_changes(db);
	sqlite3_finalize(stmt);

	return changes > 0;
}

bool SqliteApplicationRepository::remove(int id)
{
	const char *sql = "DELETE FROM applications WHERE id = ?;";

	sqlite3 *db = database_.handle();
	sqlite3_stmt *stmt = nullptr;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
	{
		throw std::runtime_error("Failed to prepare DELETE statement");
	}

	sqlite3_bind_int(stmt, 1, id);

	const int rc_step = sqlite3_step(stmt);
	if (rc_step != SQLITE_DONE)
	{
		sqlite3_finalize(stmt);
		throw std::runtime_error("Failed to execute DELETE statement");
	}

	const int changes = sqlite3_changes(db);
	sqlite3_finalize(stmt);

	return changes > 0;
}

std::vector<Application> SqliteApplicationRepository::find_all()
{
	const char *sql =
		"SELECT id, company, position, location, source, status, "
		"       applied_date, last_update, notes "
		"FROM applications;";

	sqlite3 *db = database_.handle();
	sqlite3_stmt *stmt = nullptr;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
	{
		throw std::runtime_error("Failed to prepare SELECT statement");
	}

	std::vector<Application> result;

	while (true)
	{
		const int rc_step = sqlite3_step(stmt);
		if (rc_step == SQLITE_ROW)
		{
			result.push_back(map_row_to_application(stmt));
		}
		else if (rc_step == SQLITE_DONE)
		{
			break;
		}
		else
		{
			sqlite3_finalize(stmt);
			throw std::runtime_error("Failed to execute SELECT statement");
		}
	}

	sqlite3_finalize(stmt);
	return result;
}

std::optional<Application> SqliteApplicationRepository::find_by_id(int id)
{
	const char *sql =
		"SELECT id, company, position, location, source, status, "
		"       applied_date, last_update, notes "
		"FROM applications "
		"WHERE id = ?;";

	sqlite3 *db = database_.handle();
	sqlite3_stmt *stmt = nullptr;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
	{
		throw std::runtime_error("Failed to prepare SELECT by id statement");
	}

	sqlite3_bind_int(stmt, 1, id);

	const int rc_step = sqlite3_step(stmt);
	if (rc_step == SQLITE_ROW)
	{
		Application app = map_row_to_application(stmt);
		sqlite3_finalize(stmt);
		return app;
	}
	if (rc_step == SQLITE_DONE)
	{
		sqlite3_finalize(stmt);
		return std::nullopt;
	}

	sqlite3_finalize(stmt);
	throw std::runtime_error("Failed to execute SELECT by id statement");
}

std::vector<Application> SqliteApplicationRepository::find_by_status(const std::string &status)
{
	const char *sql =
		"SELECT id, company, position, location, source, status, "
		"       applied_date, last_update, notes "
		"FROM applications "
		"WHERE status = ?;";

	sqlite3 *db = database_.handle();
	sqlite3_stmt *stmt = nullptr;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
	{
		throw std::runtime_error("Failed to prepare SELECT by status statement");
	}

	sqlite3_bind_text(stmt, 1, status.c_str(), -1, SQLITE_TRANSIENT);

	std::vector<Application> result;

	while (true)
	{
		const int rc_step = sqlite3_step(stmt);
		if (rc_step == SQLITE_ROW)
		{
			result.push_back(map_row_to_application(stmt));
		}
		else if (rc_step == SQLITE_DONE)
		{
			break;
		}
		else
		{
			sqlite3_finalize(stmt);
			throw std::runtime_error("Failed to execute SELECT by status statement");
		}
	}

	sqlite3_finalize(stmt);
	return result;
}

Statistics SqliteApplicationRepository::compute_statistics()
{
	const char *sql =
		"SELECT status, COUNT(*) "
		"FROM applications "
		"GROUP BY status;";

	sqlite3 *db = database_.handle();
	sqlite3_stmt *stmt = nullptr;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
	{
		throw std::runtime_error("Failed to prepare statistics query");
	}

	Statistics stats;

	while (true)
	{
		const int rc_step = sqlite3_step(stmt);
		if (rc_step == SQLITE_ROW)
		{
			const auto *status_text = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
			const int count = sqlite3_column_int(stmt, 1);

			if (status_text != nullptr)
			{
				stats.count_by_status[status_text] = count;
			}
		}
		else if (rc_step == SQLITE_DONE)
		{
			break;
		}
		else
		{
			sqlite3_finalize(stmt);
			throw std::runtime_error("Failed to execute statistics query");
		}
	}

	sqlite3_finalize(stmt);
	return stats;
}
