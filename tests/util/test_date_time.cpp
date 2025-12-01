#include <catch2/catch_test_macros.hpp>
#include "util/date_time.h"

TEST_CASE("today_iso_returns_non_empty_string")
{
	const auto value = datetime::today_iso();

	REQUIRE_FALSE(value.empty());
}

TEST_CASE("today_iso_returns_iso_date_format")
{
	const auto value = datetime::today_iso();

	REQUIRE(value.size() == 10);
	REQUIRE(value[4] == '-');
	REQUIRE(value[7] == '-');

	for (std::size_t i = 0; i < value.size(); ++i)
	{
		if (i == 4 || i == 7)
		{
			continue;
		}

		REQUIRE(value[i] >= '0');
		REQUIRE(value[i] <= '9');
	}
}
