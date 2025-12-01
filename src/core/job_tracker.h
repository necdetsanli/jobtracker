#pragma once

#include <memory>
#include <string>
#include <vector>

#include "core/application.h"
#include "core/statistics.h"

class IApplicationRepository;

/// High-level service that applies domain rules on top of the repository.
///
/// JobTracker is responsible for:
/// - setting default values (status, dates)
/// - delegating persistence to the repository
/// - providing convenience methods like filter_by_status or compute_statistics
class JobTracker
{
public:
	/// Constructs a JobTracker owning the given repository instance.
	///
	/// \param repository Concrete repository implementation (SQLite, Postgres, etc.).
	explicit JobTracker(std::unique_ptr<IApplicationRepository> repository);

	/// Adds a new application and returns the persisted result.
	///
	/// If status is empty, "applied" is used.
	/// If applied_date / last_update are empty, they are set to today's date.
	Application add(const Application &application_template);

	/// Returns all applications.
	std::vector<Application> list_all() const;

	/// Returns all applications with the given status.
	std::vector<Application> filter_by_status(const std::string &status) const;

	/// Updates the status and note of an existing application.
	///
	/// \param id         Primary key of the application to update.
	/// \param new_status New status to set.
	/// \param note       New note to set (replaces existing notes).
	/// \return true if the application existed and was updated; false otherwise.
	bool update_status(int id, const std::string &new_status, const std::string &note);

	/// Removes an application by id.
	bool remove(int id);

	/// Computes statistics across all applications.
	Statistics compute_statistics() const;

private:
	/// Underlying storage backend.
	std::unique_ptr<IApplicationRepository> repository_;
};
