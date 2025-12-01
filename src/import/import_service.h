#pragma once

#include <cstddef>

#include "core/application.h"
#include "core/statistics.h"
#include "import/import_source.h"
#include "storage/application_repository.h"

/**
 * @brief Result of a single import run.
 */
struct ImportResult
{
	/// Total number of application templates obtained from the source.
	std::size_t total = 0;

	/// Number of applications successfully imported and persisted.
	std::size_t imported = 0;

	/// Number of applications that could not be imported.
	std::size_t failed = 0;
};

/**
 * @brief High-level service that imports applications from a source
 *        into the application repository.
 */
class ImportService
{
public:
	/**
	 * @brief Construct an ImportService.
	 *
	 * @param source      Import source providing application templates.
	 * @param repository  Repository used to persist imported applications.
	 */
	ImportService(IImportSource &source, IApplicationRepository &repository);

	/**
	 * @brief Fetch applications from the source and persist them once.
	 *
	 * @return ImportResult structure with aggregated counts.
	 */
	ImportResult run_once();

private:
	IImportSource &source_;
	IApplicationRepository &repository_;
};
