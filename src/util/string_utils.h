#pragma once

#include <string>
#include <vector>

/// String-related helper functions.
///
/// All functions in this namespace are small, pure utilities that operate on
/// std::string values and do not perform any I/O.
namespace string_utils
{
	/// Splits the given string into parts using a single-character delimiter.
	///
	/// Consecutive delimiters will produce empty segments. The last segment is
	/// always pushed, even if empty.
	///
	/// \param input     String to split.
	/// \param delimiter Delimiter character to split on.
	/// \return A vector of substrings in the original order.
	std::vector<std::string> split(const std::string &input, char delimiter);

	/// Removes leading and trailing whitespace from the given string.
	///
	/// Whitespace characters are detected using std::isspace with an
	/// unsigned-char cast. If the string consists only of whitespace,
	/// an empty string is returned.
	///
	/// \param input String to trim.
	/// \return A copy of the input without leading and trailing whitespace.
	std::string trim(const std::string &input);

	/// Converts all characters in the given string to lowercase.
	///
	/// Locale is not customized; std::tolower is used with an unsigned-char
	/// cast for each character.
	///
	/// \param input String to convert.
	/// \return A lowercase copy of the input.
	std::string to_lower(const std::string &input);
}
