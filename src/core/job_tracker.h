#pragma once

#include <vector>

#include "core/application.h"
#include "core/application_repository.h"

/**
 * @brief High-level facade for managing job applications.
 *
 * JobTracker coordinates between the domain model (Application) and
 * the persistence layer (IApplicationRepository). It exposes high-level
 * operations used by the CLI and GUI layers.
 */
class JobTracker
{
public:
	/**
	 * @brief Construct a JobTracker that uses the given repository.
	 *
	 * @param repository The application repository used for persistence.
	 */
	explicit JobTracker(IApplicationRepository &repository);

	/**
	 * @brief Add a new application to the tracker.
	 *
	 * @param app The application to be added.
	 */
	void add_application(const Application &app);

	/**
	 * @brief Retrieve all tracked applications.
	 *
	 * @return A vector containing all applications.
	 */
	[[nodiscard]] std::vector<Application> list_all() const;

	/**
	 * @brief Find applications by status.
	 *
	 * @param status Status filter (e.g. "applied", "interview").
	 * @return A vector of matching applications.
	 */
	[[nodiscard]] std::vector<Application> find_by_status(const std::string &status) const;

private:
	/// Underlying repository used to persist and load applications.
	IApplicationRepository &repository_;
};
