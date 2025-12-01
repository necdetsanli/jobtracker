#pragma once

#include <string>
#include <chrono>

/**
 * @brief Represents a simplified email message fetched from an IMAP server.
 */
struct EmailMessage
{
	std::string id;             ///< Provider-specific unique identifier or UID
	std::string from;
	std::string to;
	std::string subject;
	std::string body_text;      ///< Plain-text body (HTML already stripped, if possible)
	std::chrono::system_clock::time_point date;
};
