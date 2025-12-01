#pragma once

#include <string>
#include <vector>

#include "import/email_message.h"

/**
 * @brief Abstraction for an email client used by import sources.
 *
 * This interface allows us to implement real IMAP/Gmail clients
 * and test doubles without coupling the import logic to any specific library.
 */
class IEmailClient
{
public:
	virtual ~IEmailClient() = default;

	virtual void connect() = 0;
	virtual void disconnect() = 0;

	/**
	 * @brief Fetch messages matching the given search expression.
	 *
	 * The expression is client-specific (e.g. IMAP search syntax).
	 */
	virtual std::vector<EmailMessage> fetch_messages(const std::string &search_expression) = 0;
};
