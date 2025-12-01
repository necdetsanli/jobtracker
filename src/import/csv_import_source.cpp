/// \file
/// \brief CSV-based implementation of IImportSource.

#include "import/csv_import_source.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

#include "util/string_utils.h"

CsvImportSource::CsvImportSource(const std::string &path, char delimiter)
	: path_(path)
	, delimiter_(delimiter)
{
}

std::vector<Application> CsvImportSource::fetch_applications()
{
	std::ifstream file(path_);
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open CSV file: " + path_);
	}

	std::string header_line;
	if (!std::getline(file, header_line))
	{
		// Empty file – nothing to import.
		return {};
	}

	const auto header_parts = string_utils::split(header_line, delimiter_);

	// Map normalized column names -> index.
	std::unordered_map<std::string, std::size_t> column_index;

	for (std::size_t i = 0; i < header_parts.size(); ++i)
	{
		const auto name_trimmed = string_utils::trim(header_parts[i]);
		const auto name_lower = string_utils::to_lower(name_trimmed);
		if (!name_lower.empty())
		{
			column_index[name_lower] = i;
		}
	}

	auto get_column = [&](const std::vector<std::string> &cols, const std::string &name) -> std::string
	{
		const auto it = column_index.find(name);
		if (it == column_index.end())
		{
			return {};
		}

		const std::size_t idx = it->second;
		if (idx >= cols.size())
		{
			return {};
		}

		return string_utils::trim(cols[idx]);
	};

	std::vector<Application> result;
	std::string line;

	while (std::getline(file, line))
	{
		if (line.empty())
		{
			continue;
		}

		const auto parts = string_utils::split(line, delimiter_);
		if (parts.empty())
		{
			continue;
		}

		Application app;

		app.company = get_column(parts, "company");
		app.position = get_column(parts, "position");
		app.location = get_column(parts, "location");
		app.source = get_column(parts, "source");
		app.status = get_column(parts, "status");
		app.applied_date = get_column(parts, "applied_date");
		app.last_update = get_column(parts, "last_update");
		app.notes = get_column(parts, "notes");

		// Minimal sanity check: we require at least company + position.
		if (app.company.empty() && app.position.empty())
		{
			continue;
		}

		result.push_back(app);
	}

	return result;
}
