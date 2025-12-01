/// \file
/// \brief Implementation of the ImportService.

#include "import/import_service.h"

#include "core/application.h"
#include "core/job_tracker.h"
#include "import/import_source.h"

ImportService::ImportService(JobTracker &tracker, IImportSource &source)
	: tracker_(tracker)
	, source_(source)
{
}

ImportResult ImportService::run_once()
{
	ImportResult result;

	auto applications = source_.fetch_applications();
	result.total = applications.size();

	for (const auto &app_template : applications)
	{
		Application saved = tracker_.add(app_template);
		if (saved.id != 0)
		{
			++result.imported;
		}
	}

	return result;
}
