#include "import/import_service.h"

ImportService::ImportService(IImportSource &source, IApplicationRepository &repository)
	: source_(source)
	, repository_(repository)
{
}

ImportResult ImportService::run_once()
{
	ImportResult result{};

	const auto templates = source_.fetch_applications();
	result.total = templates.size();

	for (const auto &tmpl : templates)
	{
		try
		{
			Application app = tmpl;
			app.id = 0;

			repository_.insert(app);
			++result.imported;
		}
		catch (...)
		{
			++result.failed;
		}
	}

	return result;
}
