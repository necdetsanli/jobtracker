#pragma once

#include <optional>
#include <string>
#include <vector>

#include <sqlite3.h>

#include "storage/application_repository.h"
#include "storage/sqlite_database.h"

/**
 * @brief SQLite-based implementation of IApplicationRepository.
 *
 * This repository owns a SQLite database connection and ensures that the
 * required schema exists before performing any operations.
 */
class SqliteApplicationRepository : public IApplicationRepository
{
public:
	/**
	 * @brief Open (or create) a SQLite database at the given path and ensure the schema exists.
	 *
	 * @param database_path Path to the SQLite database file. Use ":memory:" for tests.
	 */
	explicit SqliteApplicationRepository(const std::string &database_path);

	/**
	 * @brief Insert a new application and return the persisted entity.
	 *
	 * @param application Application to insert. Its id field may be 0.
	 * @return Application with an assigned id and any storage-managed fields.
	 */
	Application insert(const Application &application) override;

	/**
	 * @brief Update an existing application.
	 *
	 * @param application Application instance with a valid id.
	 * @return true if an existing row was updated; false otherwise.
	 */
	bool update(const Application &application) override;

	/**
	 * @brief Remove an application by id.
	 *
	 * @param id Primary key of the application to remove.
	 * @return true if a row was deleted; false if no matching id existed.
	 */
	bool remove(int id) override;

	/**
	 * @brief Retrieve all applications stored in the database.
	 *
	 * @return A vector containing all applications.
	 */
	std::vector<Application> find_all() override;

	/**
	 * @brief Find a single application by id.
	 *
	 * @param id Primary key of the application to look up.
	 * @return An optional Application; std::nullopt if no match is found.
	 */
	std::optional<Application> find_by_id(int id) override;

	/**
	 * @brief Retrieve all applications that have the given status.
	 *
	 * @param status Status filter (e.g. "applied", "interview").
	 * @return A vector of applications with the given status.
	 */
	std::vector<Application> find_by_status(const std::string &status) override;

	/**
	 * @brief Compute aggregated statistics across all applications.
	 *
	 * @return Statistics structure containing aggregated counts.
	 */
	Statistics compute_statistics() override;

private:
	/// Low-level SQLite database wrapper that manages the connection handle.
	SqliteDatabase database_;

	/**
	 * @brief Ensure that the required database schema exists.
	 *
	 * This method creates tables and indexes if they are missing.
	 */
	void ensure_schema();

	/**
	 * @brief Map the current row of a prepared SQLite statement to an Application object.
	 *
	 * @param stmt Prepared SQLite statement positioned on a valid row.
	 * @return Application populated from the current row.
	 */
	Application map_row_to_application(sqlite3_stmt *stmt) const;
};
