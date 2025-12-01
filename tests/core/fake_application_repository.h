#pragma once

#include <algorithm>
#include <optional>
#include <string>
#include <vector>

#include "storage/application_repository.h"

/**
 * @brief Simple in-memory repository implementation used only in unit tests.
 *
 * This fake repository stores applications in memory and assigns incremental
 * ids to newly inserted applications when needed. It never touches a real
 * database and is intended purely for unit testing.
 */
class FakeApplicationRepository : public IApplicationRepository
{
public:
	/**
	 * @brief Insert a new application into the in-memory store.
	 *
	 * If the application's id is 0, a new id is assigned automatically.
	 *
	 * @param application Application to insert.
	 * @return Application with a valid id.
	 */
	Application insert(const Application &application) override
	{
		Application copy = application;

		if (copy.id == 0)
		{
			copy.id = next_id_;
			++next_id_;
		}

		store_.push_back(copy);
		return copy;
	}

	/**
	 * @brief Update an existing application in the in-memory store.
	 *
	 * @param application Application instance with a valid id.
	 * @return true if a matching item was found and updated; false otherwise.
	 */
	bool update(const Application &application) override
	{
		for (auto &item : store_)
		{
			if (item.id == application.id)
			{
				item = application;
				return true;
			}
		}
		return false;
	}

	/**
	 * @brief Remove an application by id.
	 *
	 * @param id Id of the application to remove.
	 * @return true if an item was removed; false if no matching id existed.
	 */
	bool remove(int id) override
	{
		auto it = std::remove_if(
			store_.begin(),
			store_.end(),
			[id](const Application &app)
			{
				return app.id == id;
			});

		if (it == store_.end())
		{
			return false;
		}

		store_.erase(it, store_.end());
		return true;
	}

	/**
	 * @brief Retrieve all applications from the in-memory store.
	 *
	 * @return A copy of the internal application list.
	 */
	std::vector<Application> find_all() override
	{
		return store_;
	}

	/**
	 * @brief Find a single application by id.
	 *
	 * @param id Id of the application to look up.
	 * @return An optional Application; std::nullopt if not found.
	 */
	std::optional<Application> find_by_id(int id) override
	{
		for (const auto &item : store_)
		{
			if (item.id == id)
			{
				return item;
			}
		}
		return std::nullopt;
	}

	/**
	 * @brief Retrieve all applications that have the given status.
	 *
	 * @param status Status filter (e.g. "applied", "interview").
	 * @return A vector of applications with the given status.
	 */
	std::vector<Application> find_by_status(const std::string &status) override
	{
		std::vector<Application> result;

		for (const auto &item : store_)
		{
			if (item.status == status)
			{
				result.push_back(item);
			}
		}

		return result;
	}

	/**
	 * @brief Compute aggregated statistics across all in-memory applications.
	 *
	 * @return Statistics structure containing aggregated counts by status.
	 */
	Statistics compute_statistics() override
	{
		Statistics stats;

		for (const auto &item : store_)
		{
			++stats.count_by_status[item.status];
		}

		return stats;
	}

private:
	/// Next id to assign to applications whose id is initially 0.
	int next_id_ = 1;

	/// In-memory storage for application objects used by tests.
	std::vector<Application> store_;
};
