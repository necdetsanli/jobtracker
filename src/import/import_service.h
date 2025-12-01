#pragma once

#include <cstddef>

class JobTracker;
class IImportSource;

/// Result summary of a single import run.
struct ImportResult
{
	/// Total number of applications provided by the import source.
	std::size_t total = 0;

	/// Number of applications successfully stored by the tracker.
	std::size_t imported = 0;
};

/// Coordinates importing job applications from an external source into the tracker.
///
/// This service does not know *how* the data is fetched (IImportSource),
/// and it does not know *how* it is persisted (JobTracker + repository).
class ImportService
{
public:
	/// Constructs a new ImportService instance.
	///
	/// \param tracker JobTracker used to persist applications.
	/// \param source  External import source providing Application templates.
	ImportService(JobTracker &tracker, IImportSource &source);

	/// Runs a single import pass.
	///
	/// The service asks the source for applications, then passes them one by one
	/// to JobTracker::add(). Basic counters are returned in an ImportResult.
	///
	/// \return Summary of the import operation.
	ImportResult run_once();

private:
	JobTracker &tracker_;
	IImportSource &source_;
};
