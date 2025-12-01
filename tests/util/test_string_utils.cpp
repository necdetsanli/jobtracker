#include <catch2/catch_test_macros.hpp>

#include "util/string_utils.h"

/// Verifies that split() splits the string into multiple parts using
/// the given delimiter.
TEST_CASE("split_splits_string_by_delimiter")
{
	const std::string input = "a,b,c";
	const char delimiter = ',';

	const auto parts = string_utils::split(input, delimiter);

	REQUIRE(parts.size() == 3);
	REQUIRE(parts[0] == "a");
	REQUIRE(parts[1] == "b");
	REQUIRE(parts[2] == "c");
}

/// Verifies that split() returns the entire string as a single element
/// when the delimiter does not occur in the input.
TEST_CASE("split_returns_whole_string_when_delimiter_not_found")
{
	const std::string input = "hello";
	const char delimiter = ',';

	const auto parts = string_utils::split(input, delimiter);

	REQUIRE(parts.size() == 1);
	REQUIRE(parts[0] == "hello");
}

/// Verifies that trim() removes leading and trailing whitespace characters.
TEST_CASE("trim_removes_leading_and_trailing_whitespace")
{
	const std::string input = "  \t hello \n";

	const auto result = string_utils::trim(input);

	REQUIRE(result == "hello");
}

/// Verifies that to_lower() converts all characters to lowercase.
TEST_CASE("to_lower_converts_all_characters_to_lowercase")
{
	const std::string input = "HeLLo WoRLD";

	const auto result = string_utils::to_lower(input);

	REQUIRE(result == "hello world");
}
