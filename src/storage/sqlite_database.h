#pragma once

#include <string>

struct sqlite3;

/// RAII wrapper around a SQLite database connection.
///
/// This class is responsible for opening and closing the database and for
/// executing simple non-query SQL statements.
class SqliteDatabase
{
public:
	/// Opens a SQLite database at the given path.
	///
	/// \param path Path to the database file. Use ":memory:" for an in-memory database.
	explicit SqliteDatabase(const std::string &path);

	/// Closes the database if it is open.
	~SqliteDatabase();

	SqliteDatabase(const SqliteDatabase &) = delete;
	SqliteDatabase &operator=(const SqliteDatabase &) = delete;

	SqliteDatabase(SqliteDatabase &&other) noexcept;
	SqliteDatabase &operator=(SqliteDatabase &&other) noexcept;

	/// Returns the raw sqlite3 handle.
	sqlite3 *handle() const;

	/// Executes a SQL statement that does not return any rows.
	///
	/// This is useful for schema creation or simple INSERT/UPDATE without
	/// reading the result.
	///
	/// \param sql SQL statement to execute.
	/// \throws std::runtime_error if the execution fails.
	void execute_non_query(const std::string &sql);

private:
	sqlite3 *db_ = nullptr;
};
