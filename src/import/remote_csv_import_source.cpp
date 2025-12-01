#include "import/remote_csv_import_source.h"

#include <sstream>
#include <unordered_map>

#include "util/string_utils.h"

RemoteCsvImportSource::RemoteCsvImportSource(IHttpClient &http_client, const RemoteCsvConfig &config)
	: http_client_(http_client)
	, config_(config)
{
}

std::vector<Application> RemoteCsvImportSource::fetch_applications()
{
	std::vector<Application> result;

	const HttpResponse response = http_client_.get(config_.url);

	// Soft-fail on HTTP error: return empty list, caller can log.
	if (response.status_code != 200)
	{
		return result;
	}

	if (response.body.empty())
	{
		return result;
	}

	std::istringstream stream(response.body);

	std::string header_line;
	if (!std::getline(stream, header_line))
	{
		return result;
	}

	const auto header_parts = string_utils::split(header_line, config_.delimiter);
	if (header_parts.empty())
	{
		return result;
	}

	std::unordered_map<std::string, std::size_t> column_index;

	for (std::size_t i = 0; i < header_parts.size(); ++i)
	{
		const auto trimmed = string_utils::trim(header_parts[i]);
		const auto lower = string_utils::to_lower(trimmed);
		column_index[lower] = i;
	}

	auto get_column = [&](const std::vector<std::string> &parts, const char *name) -> std::string
	{
		const auto it = column_index.find(name);
		if (it == column_index.end())
		{
			return {};
		}

		const auto idx = it->second;
		if (idx >= parts.size())
		{
			return {};
		}

		return string_utils::trim(parts[idx]);
	};

	std::string line;
	while (std::getline(stream, line))
	{
		if (line.empty())
		{
			continue;
		}

		const auto parts = string_utils::split(line, config_.delimiter);
		if (parts.empty())
		{
			continue;
		}

		Application app{};

		app.company = get_column(parts, "company");
		app.position = get_column(parts, "position");
		app.location = get_column(parts, "location");
		app.source = get_column(parts, "source");
		app.status = get_column(parts, "status");
		app.applied_date = get_column(parts, "applied_date");
		app.last_update = get_column(parts, "last_update");
		app.notes = get_column(parts, "notes");

		// Minimal sanity check: require at least company + position.
		if (app.company.empty() || app.position.empty())
		{
			continue;
		}

		result.push_back(app);
	}

	return result;
}
