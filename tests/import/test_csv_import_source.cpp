#include <catch2/catch_test_macros.hpp>
#include <fstream>

#include "import/csv_import_source.h"

TEST_CASE("CsvImportSource_reads_applications_from_csv_file")
{
	const std::string file_name = "test_csv_import_source.csv";

	// Create a temporary CSV file in the current working directory.
	{
		std::ofstream out(file_name);
		REQUIRE(out.is_open());

		out << "company,position,location,source,status,applied_date,last_update,notes\n";
		out << "ACME,C++ Developer,Remote,linkedin,applied,2025-01-01,2025-01-01,First application\n";
		out << "Beta,DevOps Engineer,Berlin,email,interview,2025-01-02,2025-01-03,Phone screen done\n";
	}

	CsvImportSource source(file_name);
	const auto apps = source.fetch_applications();

	REQUIRE(apps.size() == 2);

	REQUIRE(apps[0].company == "ACME");
	REQUIRE(apps[0].position == "C++ Developer");
	REQUIRE(apps[0].location == "Remote");
	REQUIRE(apps[0].source == "linkedin");
	REQUIRE(apps[0].status == "applied");
	REQUIRE(apps[0].applied_date == "2025-01-01");
	REQUIRE(apps[0].last_update == "2025-01-01");
	REQUIRE(apps[0].notes == "First application");

	REQUIRE(apps[1].company == "Beta");
	REQUIRE(apps[1].position == "DevOps Engineer");
	REQUIRE(apps[1].location == "Berlin");
	REQUIRE(apps[1].source == "email");
	REQUIRE(apps[1].status == "interview");
	REQUIRE(apps[1].applied_date == "2025-01-02");
	REQUIRE(apps[1].last_update == "2025-01-03");
	REQUIRE(apps[1].notes == "Phone screen done");
}

TEST_CASE("CsvImportSource_throws_when_file_does_not_exist")
{
	const std::string missing_file = "this_file_does_not_exist.csv";

	CsvImportSource source(missing_file);

	REQUIRE_THROWS_AS(source.fetch_applications(), std::runtime_error);
}

TEST_CASE("CsvImportSource_returns_empty_vector_for_header_only_file")
{
	const std::string file_name = "test_csv_import_source_header_only.csv";

	{
		std::ofstream out(file_name);
		REQUIRE(out.is_open());

		out << "company,position,location,source,status,applied_date,last_update,notes\n";
		// No data rows.
	}

	CsvImportSource source(file_name);
	const auto apps = source.fetch_applications();

	REQUIRE(apps.empty());
}
