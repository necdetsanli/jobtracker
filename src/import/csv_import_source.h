#pragma once

#include <string>
#include <vector>

#include "core/application.h"
#include "import/import_source.h"

/**
 * @brief Import source that reads job applications from a CSV file.
 *
 * The CSV file is expected to have a header row. Column names are matched
 * in a case-insensitive way. Unknown columns are ignored.
 *
 * Supported column names:
 *   - company
 *   - position
 *   - location
 *   - source
 *   - status
 *   - applied_date
 *   - last_update
 *   - notes
 */
class CsvImportSource : public IImportSource
{
public:
	/**
	 * @brief Construct a CsvImportSource for the given file path.
	 *
	 * @param path      Path to the CSV file.
	 * @param delimiter Delimiter character used to separate columns (default: ',').
	 */
	explicit CsvImportSource(const std::string &path, char delimiter = ',');

	/**
	 * @brief Read the CSV file and convert each row into an Application.
	 *
	 * @return A vector of Application objects parsed from the CSV file.
	 */
	std::vector<Application> fetch_applications() override;

private:
	/// Path to the CSV file.
	std::string path_;

	/// Delimiter character used to separate columns.
	char delimiter_;
};
