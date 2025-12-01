#pragma once

#include <string>
#include <vector>

#include "core/application.h"
#include "import/import_source.h"
#include "import/http_client.h"

/**
 * @brief Configuration for RemoteCsvImportSource.
 */
struct RemoteCsvConfig
{
	/// URL of the CSV document to fetch.
	std::string url;

	/// Delimiter character used to split columns (default: ',').
	char delimiter = ',';
};

/**
 * @brief Import source that fetches a CSV document over HTTP.
 *
 * The fetched body is parsed using the same conventions as CsvImportSource.
 */
class RemoteCsvImportSource : public IImportSource
{
public:
	/**
	 * @brief Construct a RemoteCsvImportSource.
	 *
	 * @param http_client HTTP client used to perform the GET request.
	 * @param config      Configuration describing which URL to fetch and how to parse it.
	 */
	RemoteCsvImportSource(IHttpClient &http_client, const RemoteCsvConfig &config);

	/**
	 * @brief Fetch applications from the remote CSV.
	 *
	 * If the HTTP request fails or responds with a non-200 status, this
	 * returns an empty vector.
	 */
	std::vector<Application> fetch_applications() override;

private:
	/// HTTP client used to fetch the CSV document.
	IHttpClient &http_client_;

	/// Configuration describing URL and delimiter.
	RemoteCsvConfig config_;
};
