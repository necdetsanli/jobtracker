#pragma once

#include <vector>

#include "core/application.h"

/// Abstraction for external systems that can provide job applications.
///
/// Implementations might talk to e-mail providers, job boards, or other
/// APIs and convert remote data into Application objects.
class IImportSource
{
public:
	/// Virtual destructor to allow deletion through interface pointer.
	virtual ~IImportSource() = default;

	/// Fetches job applications from the external source.
	///
	/// Returned Application objects are usually "templates": they may provide
	/// values such as company and position, while leaving status and date
	/// fields empty so that JobTracker can apply defaults.
	///
	/// \return A list of applications obtained from the external system.
	virtual std::vector<Application> fetch_applications() = 0;
};
