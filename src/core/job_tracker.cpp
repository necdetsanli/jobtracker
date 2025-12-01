#include "core/job_tracker.h"

JobTracker::JobTracker(IApplicationRepository &repository)
	: repository_(repository)
{
}

Application JobTracker::add(const Application &application_template)
{
	Application app = application_template;

	// Ensure the storage layer assigns a fresh id.
	app.id = 0;

	// Provide a sensible default status if none is set.
	if (app.status.empty())
	{
		app.status = "applied";
	}

	return repository_.insert(app);
}

std::vector<Application> JobTracker::list_all() const
{
	return repository_.find_all();
}

std::vector<Application> JobTracker::filter_by_status(const std::string &status) const
{
	return repository_.find_by_status(status);
}

bool JobTracker::update_status(int id, const std::string &new_status, const std::string &note)
{
	auto existing = repository_.find_by_id(id);
	if (!existing)
	{
		return false;
	}

	Application app = *existing;
	app.status = new_status;

	if (!note.empty())
	{
		if (!app.notes.empty())
		{
			app.notes += "\n";
		}
		app.notes += note;
	}

	return repository_.update(app);
}

bool JobTracker::remove(int id)
{
	return repository_.remove(id);
}

Statistics JobTracker::compute_statistics() const
{
	return repository_.compute_statistics();
}
