#pragma once

#include <optional>
#include <string>
#include <vector>

#include "core/application.h"
#include "core/statistics.h"

/// Abstract repository interface for storing and retrieving job applications.
///
/// This interface allows different storage backends (SQLite, PostgreSQL, etc.)
/// to be plugged in behind the same API.
class IApplicationRepository
{
public:
	/// Virtual destructor to allow proper cleanup through interface pointer.
	virtual ~IApplicationRepository() = default;

	/// Inserts a new application and returns the persisted entity.
	///
	/// \param application Application to insert. Its id field may be 0.
	/// \return Application with assigned id and any storage-managed fields.
	virtual Application insert(const Application &application) = 0;

	/// Updates an existing application.
	///
	/// \param application Application instance with a valid id.
	/// \return true if an existing row was updated; false otherwise.
	virtual bool update(const Application &application) = 0;

	/// Removes an application by id.
	///
	/// \param id Primary key of the application to remove.
	/// \return true if a row was deleted; false if no matching id existed.
	virtual bool remove(int id) = 0;

	/// Returns all applications.
	virtual std::vector<Application> find_all() = 0;

	/// Finds a single application by id.
	virtual std::optional<Application> find_by_id(int id) = 0;

	/// Returns all applications that have the given status.
	virtual std::vector<Application> find_by_status(const std::string &status) = 0;

	/// Computes aggregated statistics across all applications.
	virtual Statistics compute_statistics() = 0;
};
