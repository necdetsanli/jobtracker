#include <catch2/catch_test_macros.hpp>

#include "import/remote_csv_import_source.h"
#include "tests/import/fake_http_client.h"

TEST_CASE("RemoteCsvImportSource imports applications from HTTP CSV")
{
	FakeHttpClient http_client;

	const std::string url = "https://example.com/jobs.csv";
	const std::string csv_data =
		"company,position,location,status,notes\n"
		"ACME,Backend Engineer,Remote,applied,First\n"
		"Globex,DevOps Engineer,Berlin,interview,Second\n";

	HttpResponse response{};
	response.status_code = 200;
	response.body = csv_data;

	http_client.set_response(url, response);

	RemoteCsvConfig config{};
	config.url = url;
	config.delimiter = ',';

	RemoteCsvImportSource source(http_client, config);

	const auto apps = source.fetch_applications();

	REQUIRE(apps.size() == 2);

	REQUIRE(apps[0].company == "ACME");
	REQUIRE(apps[0].position == "Backend Engineer");
	REQUIRE(apps[0].location == "Remote");
	REQUIRE(apps[0].status == "applied");
	REQUIRE(apps[0].notes == "First");

	REQUIRE(apps[1].company == "Globex");
	REQUIRE(apps[1].position == "DevOps Engineer");
	REQUIRE(apps[1].location == "Berlin");
	REQUIRE(apps[1].status == "interview");
	REQUIRE(apps[1].notes == "Second");
}

TEST_CASE("RemoteCsvImportSource returns empty list on HTTP error")
{
	FakeHttpClient http_client;

	const std::string url = "https://example.com/jobs.csv";

	HttpResponse response{};
	response.status_code = 500;
	response.body = "Internal Server Error";

	http_client.set_response(url, response);

	RemoteCsvConfig config{};
	config.url = url;
	config.delimiter = ',';

	RemoteCsvImportSource source(http_client, config);

	const auto apps = source.fetch_applications();

	REQUIRE(apps.empty());
}

TEST_CASE("RemoteCsvImportSource returns empty list on empty body")
{
	FakeHttpClient http_client;

	const std::string url = "https://example.com/jobs.csv";

	HttpResponse response{};
	response.status_code = 200;
	response.body = "";

	http_client.set_response(url, response);

	RemoteCsvConfig config{};
	config.url = url;
	config.delimiter = ',';

	RemoteCsvImportSource source(http_client, config);

	const auto apps = source.fetch_applications();

	REQUIRE(apps.empty());
}
