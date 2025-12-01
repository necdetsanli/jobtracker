#pragma once

#include <string>

/// Date and time helper functions.
namespace datetime
{
	/// Returns today's date in ISO 8601 format (YYYY-MM-DD).
	///
	/// The function uses the local time zone of the system.
	///
	/// \return A string representing today's date, for example "2025-01-01".
	std::string today_iso();
}
