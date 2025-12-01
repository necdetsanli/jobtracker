/// \file
/// \brief Implementation of the SqliteDatabase RAII helper.

#include "storage/sqlite_database.h"

#include <sqlite3.h>

#include <stdexcept>

SqliteDatabase::SqliteDatabase(const std::string &path)
{
	const int rc = sqlite3_open(path.c_str(), &db_);
	if (rc != SQLITE_OK)
	{
		std::string message = "Failed to open SQLite database: ";
		message += sqlite3_errmsg(db_);
		sqlite3_close(db_);
		db_ = nullptr;
		throw std::runtime_error(message);
	}
}

SqliteDatabase::~SqliteDatabase()
{
	if (db_ != nullptr)
	{
		sqlite3_close(db_);
		db_ = nullptr;
	}
}

SqliteDatabase::SqliteDatabase(SqliteDatabase &&other) noexcept
	: db_(other.db_)
{
	other.db_ = nullptr;
}

SqliteDatabase &SqliteDatabase::operator=(SqliteDatabase &&other) noexcept
{
	if (this != &other)
	{
		if (db_ != nullptr)
		{
			sqlite3_close(db_);
		}
		db_ = other.db_;
		other.db_ = nullptr;
	}
	return *this;
}

sqlite3 *SqliteDatabase::handle() const
{
	return db_;
}

void SqliteDatabase::execute_non_query(const std::string &sql)
{
	char *error_message = nullptr;
	const int rc = sqlite3_exec(
		db_,
		sql.c_str(),
		nullptr,
		nullptr,
		&error_message
	);

	if (rc != SQLITE_OK)
	{
		std::string message = "SQLite execute_non_query failed: ";
		if (error_message != nullptr)
		{
			message += error_message;
			sqlite3_free(error_message);
		}
		throw std::runtime_error(message);
	}
}
