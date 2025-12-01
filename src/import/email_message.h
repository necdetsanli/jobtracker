#pragma once

#include <string>

/**
 * @brief Represents a simplified email message used by import sources.
 */
struct EmailMessage
{
	/// Provider-specific unique identifier or UID.
	std::string id;

	/// Subject line of the email.
	std::string subject;

	/// Sender address (as provided by the mail provider).
	std::string from;

	/// Primary recipient address.
	std::string to;

	/// Plain-text body (HTML already stripped, if possible).
	std::string body_text;

	/// Date string as provided by the mail provider.
	std::string date;
};
