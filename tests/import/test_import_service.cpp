#include <catch2/catch_test_macros.hpp>

#include "core/job_tracker.h"
#include "core/application.h"
#include "core/statistics.h"
#include "import/import_service.h"
#include "import/import_source.h"
#include "import/fake_import_source.h"
#include "core/fake_application_repository.h"

TEST_CASE("ImportService_imports_all_applications_from_source")
{
	auto repository = std::make_unique<FakeApplicationRepository>();
	JobTracker tracker(std::move(repository));

	FakeImportSource source;

	Application a1;
	a1.company = "ACME";
	a1.position = "C++ Developer";

	Application a2;
	a2.company = "Beta";
	a2.position = "DevOps Engineer";

	source.add_application_template(a1);
	source.add_application_template(a2);

	ImportService service(tracker, source);

	ImportResult result = service.run_once();

	REQUIRE(result.total == 2);
	REQUIRE(result.imported == 2);

	const auto all = tracker.list_all();
	REQUIRE(all.size() == 2);
	REQUIRE(all[0].company == "ACME");
	REQUIRE(all[1].company == "Beta");
}

TEST_CASE("ImportService_relies_on_jobtracker_to_set_default_status_and_dates")
{
	auto repository = std::make_unique<FakeApplicationRepository>();
	JobTracker tracker(std::move(repository));

	FakeImportSource source;

	Application template_app;
	template_app.company = "ACME";
	template_app.position = "C++ Developer";

	source.add_application_template(template_app);

	ImportService service(tracker, source);
	ImportResult result = service.run_once();

	REQUIRE(result.total == 1);
	REQUIRE(result.imported == 1);

	const auto apps = tracker.list_all();
	REQUIRE(apps.size() == 1);

	const auto &saved = apps[0];
	REQUIRE(saved.company == "ACME");
	REQUIRE(saved.position == "C++ Developer");
	REQUIRE(saved.status == "applied");
	REQUIRE_FALSE(saved.applied_date.empty());
	REQUIRE_FALSE(saved.last_update.empty());
}
