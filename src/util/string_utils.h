#pragma once

#include <string>
#include <vector>

/**
 * @brief String-related helper functions.
 */
namespace string_utils
{
	/**
	 * @brief Split a string into parts using the given delimiter.
	 *
	 * Consecutive delimiters produce empty segments. Trailing empty segments
	 * are preserved.
	 *
	 * @param input     Input string to split.
	 * @param delimiter Delimiter character.
	 * @return A vector of substrings.
	 */
	std::vector<std::string> split(const std::string &input, char delimiter);

	/**
	 * @brief Remove leading and trailing whitespace characters from a string.
	 *
	 * @param input Input string.
	 * @return A trimmed copy of the input.
	 */
	std::string trim(const std::string &input);

	/**
	 * @brief Convert all characters in a string to lowercase.
	 *
	 * @param input Input string.
	 * @return Lowercased copy of the input.
	 */
	std::string to_lower(const std::string &input);
}
