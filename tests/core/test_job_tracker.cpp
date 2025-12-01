#include <catch2/catch_test_macros.hpp>

#include "core/job_tracker.h"
#include "core/application.h"
#include "util/date_time.h"
#include "core/statistics.h"
#include "core/fake_application_repository.h"

TEST_CASE("add_sets_default_status_and_dates_when_missing")
{
	auto repository = std::make_unique<FakeApplicationRepository>();
	JobTracker tracker(std::move(repository));

	Application template_app;
	template_app.company = "ACME";
	template_app.position = "C++ Developer";

	const auto today = datetime::today_iso();

	Application saved = tracker.add(template_app);

	REQUIRE(saved.id != 0);
	REQUIRE(saved.company == "ACME");
	REQUIRE(saved.position == "C++ Developer");
	REQUIRE(saved.status == "applied");
	REQUIRE(saved.applied_date == today);
	REQUIRE(saved.last_update == today);
}

TEST_CASE("filter_by_status_returns_only_matching_applications")
{
	auto repository = std::make_unique<FakeApplicationRepository>();
	JobTracker tracker(std::move(repository));

	Application a1;
	a1.company = "ACME";
	a1.position = "C++ Developer";
	tracker.add(a1);

	Application a2;
	a2.company = "Beta";
	a2.position = "Backend Engineer";
	tracker.add(a2);

	Application a3;
	a3.company = "Gamma";
	a3.position = "DevOps Engineer";
	Application saved3 = tracker.add(a3);

	tracker.update_status(saved3.id, "interview", "");

	const auto applied = tracker.filter_by_status("applied");
	const auto interview = tracker.filter_by_status("interview");

	REQUIRE(applied.size() == 2);
	REQUIRE(interview.size() == 1);
	REQUIRE(interview[0].company == "Gamma");
}

TEST_CASE("update_status_updates_status_notes_and_last_update")
{
	auto repository = std::make_unique<FakeApplicationRepository>();
	JobTracker tracker(std::move(repository));

	Application app;
	app.company = "ACME";
	app.position = "C++ Developer";
	Application saved = tracker.add(app);

	const bool updated = tracker.update_status(
		saved.id,
		"interview",
		"Phone screen scheduled"
	);

	REQUIRE(updated);

	const auto all = tracker.list_all();
	REQUIRE(all.size() == 1);

	const auto &updated_app = all[0];

	REQUIRE(updated_app.status == "interview");
	REQUIRE(updated_app.notes == "Phone screen scheduled");
	REQUIRE_FALSE(updated_app.last_update.empty());
}
