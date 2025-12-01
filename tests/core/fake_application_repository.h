#pragma once

#include <algorithm>
#include <optional>
#include <vector>

#include "storage/application_repository.h"

/// Simple in-memory repository implementation used only in unit tests.
class FakeApplicationRepository : public IApplicationRepository
{
public:
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

	std::vector<Application> find_all() override
	{
		return store_;
	}

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
	int next_id_ = 1;
	std::vector<Application> store_;
};
