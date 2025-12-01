#pragma once

#include <vector>

#include "import/import_source.h"

/// Simple in-memory import source used only in unit tests.
class FakeImportSource : public IImportSource
{
public:
	std::vector<Application> fetch_applications() override
	{
		return applications_;
	}

	/// Adds an application template that will be returned on the next fetch.
	void add_application_template(const Application &app)
	{
		applications_.push_back(app);
	}

private:
	std::vector<Application> applications_;
};
