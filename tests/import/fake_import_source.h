#pragma once

#include <vector>

#include "core/application.h"
#include "import/import_source.h"

/**
 * @brief Simple in-memory import source used only in unit tests.
 *
 * This fake import source returns a pre-populated list of Application objects
 * instead of talking to a real external system.
 */
class FakeImportSource : public IImportSource
{
public:
	/**
	 * @brief Return all application templates currently stored in memory.
	 *
	 * @return A vector of Application objects.
	 */
	std::vector<Application> fetch_applications() override
	{
		return applications_;
	}

	/**
	 * @brief Add an application template that will be returned on the next fetch.
	 *
	 * @param app Application template to append to the internal list.
	 */
	void add_application_template(const Application &app)
	{
		applications_.push_back(app);
	}

private:
	/// In-memory list of application templates to be returned by fetch_applications().
	std::vector<Application> applications_;
};
