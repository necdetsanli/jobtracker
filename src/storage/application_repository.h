#pragma once

#include <optional>
#include <string>
#include <vector>

#include "core/application.h"
#include "core/statistics.h"

/**
 * @brief Abstract repository interface for storing and retrieving job applications.
 *
 * This interface allows different storage backends (SQLite, PostgreSQL, etc.)
 * to be plugged in behind the same API.
 */
class IApplicationRepository
{
public:
	/**
	 * @brief Virtual destructor to allow proper cleanup through interface pointer.
	 */
	virtual ~IApplicationRepository() = default;

	/**
	 * @brief Insert a new application and return the persisted entity.
	 *
	 * @param application Application to insert. Its id field may be 0.
	 * @return Application with an assigned id and any storage-managed fields.
	 */
	virtual Application insert(const Application &application) = 0;

	/**
	 * @brief Update an existing application.
	 *
	 * @param application Application instance with a valid id.
	 * @return true if an existing row was updated; false otherwise.
	 */
	virtual bool update(const Application &application) = 0;

	/**
	 * @brief Remove an application by id.
	 *
	 * @param id Primary key of the application to remove.
	 * @return true if a row was deleted; false if no matching id existed.
	 */
	virtual bool remove(int id) = 0;

	/**
	 * @brief Retrieve all applications.
	 *
	 * @return A vector containing all applications.
	 */
	virtual std::vector<Application> find_all() = 0;

	/**
	 * @brief Find a single application by id.
	 *
	 * @param id Primary key of the application to look up.
	 * @return An optional Application; std::nullopt if no match is found.
	 */
	virtual std::optional<Application> find_by_id(int id) = 0;

	/**
	 * @brief Retrieve all applications that have the given status.
	 *
	 * @param status Status filter (e.g. "applied", "interview").
	 * @return A vector of applications with the given status.
	 */
	virtual std::vector<Application> find_by_status(const std::string &status) = 0;

	/**
	 * @brief Compute aggregated statistics across all applications.
	 *
	 * @return Statistics structure containing aggregated counts.
	 */
	virtual Statistics compute_statistics() = 0;
};
