/// \file
/// \brief Implementation of date and time helper functions.

#include "util/date_time.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace datetime
{
	std::string today_iso()
	{
		const auto now = std::chrono::system_clock::now();
		const std::time_t t = std::chrono::system_clock::to_time_t(now);

		std::tm tm{};
		#if defined(_WIN32)
		localtime_s(&tm, &t);
		#else
		localtime_r(&t, &tm);
		#endif

		std::ostringstream oss;
		oss << std::put_time(&tm, "%Y-%m-%d");
		return oss.str();
	}
}
