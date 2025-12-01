#pragma once

#include "core/job_tracker.h"
#include "import/import_source.h"

/**
 * @brief Coordinates importing applications from a source into the tracker.
 *
 * ImportService pulls applications from an IImportSource and forwards them
 * to JobTracker. It is intentionally simple so that it can be reused from
 * the CLI or a future GUI.
 */
class ImportService
{
public:
	/**
	 * @brief Construct an ImportService with a JobTracker and an import source.
	 *
	 * @param tracker JobTracker instance used to persist imported applications.
	 * @param source Import source that provides Application objects.
	 */
	ImportService(JobTracker &tracker, IImportSource &source);

	/**
	 * @brief Import all applications from the source into the tracker.
	 *
	 * This method fetches applications from the source and adds them
	 * to the tracker one by one.
	 */
	void run_once();

private:
	/// High-level facade for persisting and managing applications.
	JobTracker &tracker_;

	/// Source that provides applications to be imported.
	IImportSource &source_;
};
