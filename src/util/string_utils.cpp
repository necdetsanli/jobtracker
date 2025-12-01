/// \file
/// \brief Implementation of string-related helper functions.

#include "util/string_utils.h"

#include <cctype>

namespace string_utils
{
	std::vector<std::string> split(const std::string &input, char delimiter)
	{
		std::vector<std::string> parts;
		std::string current;

		for (char ch : input)
		{
			if (ch == delimiter)
			{
				parts.push_back(current);
				current.clear();
			}
			else
			{
				current.push_back(ch);
			}
		}

		// Push the last segment (even if it is empty).
		parts.push_back(current);

		return parts;
	}

	std::string trim(const std::string &input)
	{
		if (input.empty())
		{
			return {};
		}

		std::size_t start = 0;
		std::size_t end = input.size() - 1;

		while (start < input.size() &&
			std::isspace(static_cast<unsigned char>(input[start])))
		{
			++start;
		}

		// If the string is entirely whitespace, return an empty string.
		if (start == input.size())
		{
			return {};
		}

		while (end > start &&
			std::isspace(static_cast<unsigned char>(input[end])))
		{
			--end;
		}

		return input.substr(start, end - start + 1);
	}

	std::string to_lower(const std::string &input)
	{
		std::string result;
		result.reserve(input.size());

		for (unsigned char ch : input)
		{
			result.push_back(static_cast<char>(std::tolower(ch)));
		}

		return result;
	}
}
