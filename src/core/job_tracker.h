#pragma once

#include <string>
#include <vector>

#include "core/application.h"
#include "core/statistics.h"
#include "storage/application_repository.h"

/**
 * @brief High-level service that manages job applications.
 *
 * This service provides a convenient API on top of the application repository
 * for common operations such as adding, listing and updating applications.
 */
class JobTracker
{
public:
	/**
	 * @brief Construct a JobTracker that operates on the given repository.
	 *
	 * @param repository Underlying repository used to persist and load applications.
	 */
	explicit JobTracker(IApplicationRepository &repository);

	/**
	 * @brief Add a new application based on a template.
	 *
	 * The template's id is ignored (it will be reset to 0 so the storage layer
	 * can assign a primary key). If status is empty, a default value such as
	 * "applied" may be used.
	 *
	 * @param application_template Template for the new application.
	 * @return The persisted Application with an assigned id.
	 */
	Application add(const Application &application_template);

	/**
	 * @brief Return all applications from the repository.
	 */
	std::vector<Application> list_all() const;

	/**
	 * @brief Return all applications that have the given status.
	 *
	 * @param status Status filter (e.g. "applied", "interview").
	 */
	std::vector<Application> filter_by_status(const std::string &status) const;

	/**
	 * @brief Update the status (and optional note) of an application.
	 *
	 * @param id         Id of the application to update.
	 * @param new_status New status value.
	 * @param note       Optional note to append to the application's notes field.
	 * @return true if the application existed and was updated; false otherwise.
	 */
	bool update_status(int id, const std::string &new_status, const std::string &note);

	/**
	 * @brief Remove an application by id.
	 *
	 * @param id Id of the application to remove.
	 * @return true if a row was removed; false if no matching id existed.
	 */
	bool remove(int id);

	/**
	 * @brief Compute aggregated statistics across all applications.
	 *
	 * @return Statistics structure with counts by status.
	 */
	Statistics compute_statistics() const;

private:
	/// Underlying repository used to persist and load applications.
	IApplicationRepository &repository_;
};
