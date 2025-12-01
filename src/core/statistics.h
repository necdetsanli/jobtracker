#pragma once

#include <map>
#include <string>

/// Aggregated statistics for job applications.
struct Statistics
{
	/// Number of applications grouped by status (e.g. "applied" -> 10).
	std::map<std::string, int> count_by_status;
};
