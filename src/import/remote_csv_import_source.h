#pragma once

#include <memory>
#include <string>
#include <vector>

#include "import/import_source.h"
#include "import/http_client.h"
#include "core/application.h"

/**
 * @brief Import source that pulls job applications from a remote CSV file.
 *
 * This class uses an IHttpClient to download a CSV document from a given URL
 * and converts each row into an Application object. It is generic and can be
 * used with any HTTP-accessible CSV, including Google Sheets published as CSV.
 */
class RemoteCsvImportSource : public IImportSource
{
public:
	/**
	 * @brief Configuration for the remote CSV import source.
	 */
	struct Config
	{
		/// URL of the CSV document to fetch.
		std::string url;
	};

	RemoteCsvImportSource(std::unique_ptr<IHttpClient> client, Config config);

	/**
	 * @brief Fetch applications from the configured remote CSV resource.
	 *
	 * @return A vector of Application objects parsed from the CSV.
	 *
	 * If the HTTP response status is not in the 2xx range, an empty
	 * vector is returned.
	 */
	std::vector<Application> fetch_applications() override;

private:
	/// HTTP client used to fetch the remote CSV document.
	std::unique_ptr<IHttpClient> client_;

	/// Configuration describing which URL to fetch.
	Config config_;

	/**
	 * @brief Parse CSV content into Application objects.
	 *
	 * @param csv_content CSV document as a string.
	 * @return Parsed applications. Invalid or incomplete rows may be skipped.
	 */
	std::vector<Application> parse_csv(const std::string &csv_content) const;
};
