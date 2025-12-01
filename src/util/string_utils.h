#pragma once

#include <string>

namespace string_utils
{
	/**
	 * @brief Trim leading and trailing whitespace from a string.
	 *
	 * @param value Input string.
	 * @return A copy of the input string without leading and trailing whitespace.
	 */
	std::string trim(const std::string &value);
}
