/// \file
/// \brief Implementation of the JobTracker service.

#include "core/job_tracker.h"

#include "storage/application_repository.h"
#include "util/date_time.h"

JobTracker::JobTracker(std::unique_ptr<IApplicationRepository> repository)
	: repository_(std::move(repository))
{
}

Application JobTracker::add(const Application &application_template)
{
	Application to_save = application_template;

	if (to_save.status.empty())
	{
		to_save.status = "applied";
	}

	const auto today = datetime::today_iso();

	if (to_save.applied_date.empty())
	{
		to_save.applied_date = today;
	}

	if (to_save.last_update.empty())
	{
		to_save.last_update = today;
	}

	return repository_->insert(to_save);
}

std::vector<Application> JobTracker::list_all() const
{
	return repository_->find_all();
}

std::vector<Application> JobTracker::filter_by_status(const std::string &status) const
{
	return repository_->find_by_status(status);
}

bool JobTracker::update_status(int id, const std::string &new_status, const std::string &note)
{
	auto existing = repository_->find_by_id(id);
	if (!existing)
	{
		return false;
	}

	existing->status = new_status;
	existing->notes = note;
	existing->last_update = datetime::today_iso();

	return repository_->update(*existing);
}

bool JobTracker::remove(int id)
{
	return repository_->remove(id);
}

Statistics JobTracker::compute_statistics() const
{
	return repository_->compute_statistics();
}
