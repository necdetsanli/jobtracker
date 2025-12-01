#pragma once

#include <string>
#include <stdexcept>

struct sqlite3;

/**
 * @brief RAII wrapper around a SQLite database connection.
 *
 * This class is responsible for opening and closing the database and for
 * executing simple non-query SQL statements.
 */
class SqliteDatabase
{
public:
	/**
	 * @brief Open a SQLite database at the given path.
	 *
	 * @param path Path to the database file. Use ":memory:" for an in-memory database.
	 *
	 * @throws std::runtime_error if the database cannot be opened.
	 */
	explicit SqliteDatabase(const std::string &path);

	/**
	 * @brief Close the database if it is open.
	 */
	~SqliteDatabase();

	SqliteDatabase(const SqliteDatabase &) = delete;
	SqliteDatabase &operator=(const SqliteDatabase &) = delete;

	/**
	 * @brief Move constructor.
	 *
	 * Transfers ownership of the database handle from another SqliteDatabase.
	 *
	 * @param other SqliteDatabase to move from.
	 */
	SqliteDatabase(SqliteDatabase &&other) noexcept;

	/**
	 * @brief Move assignment operator.
	 *
	 * Releases any currently owned database handle and takes ownership of the
	 * handle from another SqliteDatabase.
	 *
	 * @param other SqliteDatabase to move from.
	 * @return Reference to this instance.
	 */
	SqliteDatabase &operator=(SqliteDatabase &&other) noexcept;

	/**
	 * @brief Get the raw sqlite3 handle.
	 *
	 * @return Pointer to the underlying sqlite3 database handle.
	 */
	sqlite3 *handle() const;

	/**
	 * @brief Execute a SQL statement that does not return any rows.
	 *
	 * This is useful for schema creation or simple INSERT/UPDATE operations
	 * where no result set is expected.
	 *
	 * @param sql SQL statement to execute.
	 *
	 * @throws std::runtime_error if the execution fails.
	 */
	void execute_non_query(const std::string &sql);

private:
	/// Underlying sqlite3 database handle, or nullptr if not open.
	sqlite3 *db_ = nullptr;
};
