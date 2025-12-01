#include <catch2/catch_test_macros.hpp>

#include "storage/sqlite_application_repository.h"
#include "core/application.h"

TEST_CASE("sqlite_repository_inserts_and_returns_application")
{
	SqliteApplicationRepository repo(":memory:");

	Application app;
	app.company = "ACME";
	app.position = "C++ Developer";
	app.status = "applied";

	Application stored = repo.insert(app);

	REQUIRE(stored.id != 0);
	REQUIRE(stored.company == "ACME");
	REQUIRE(stored.position == "C++ Developer");
	REQUIRE(stored.status == "applied");

	const auto all = repo.find_all();
	REQUIRE(all.size() == 1);
	REQUIRE(all[0].id == stored.id);
}

TEST_CASE("sqlite_repository_filters_by_status")
{
	SqliteApplicationRepository repo(":memory:");

	Application a1;
	a1.company = "ACME";
	a1.position = "C++";
	a1.status = "applied";
	repo.insert(a1);

	Application a2;
	a2.company = "Beta";
	a2.position = "Backend";
	a2.status = "interview";
	repo.insert(a2);

	Application a3;
	a3.company = "Gamma";
	a3.position = "DevOps";
	a3.status = "applied";
	repo.insert(a3);

	const auto applied = repo.find_by_status("applied");
	const auto interview = repo.find_by_status("interview");

	REQUIRE(applied.size() == 2);
	REQUIRE(interview.size() == 1);
	REQUIRE(interview[0].company == "Beta");
}

TEST_CASE("sqlite_repository_computes_basic_statistics")
{
	SqliteApplicationRepository repo(":memory:");

	Application a1;
	a1.company = "ACME";
	a1.position = "C++";
	a1.status = "applied";
	repo.insert(a1);

	Application a2;
	a2.company = "Beta";
	a2.position = "Backend";
	a2.status = "applied";
	repo.insert(a2);

	Application a3;
	a3.company = "Gamma";
	a3.position = "DevOps";
	a3.status = "interview";
	repo.insert(a3);

	const auto stats = repo.compute_statistics();

	REQUIRE(stats.count_by_status.at("applied") == 2);
	REQUIRE(stats.count_by_status.at("interview") == 1);
}
