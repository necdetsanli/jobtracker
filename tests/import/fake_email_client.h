#pragma once

#include <vector>
#include <string>

#include "import/email_client.h"

/**
 * @brief Simple fake email client for testing ImapImportSource.
 */
class FakeEmailClient : public IEmailClient
{
public:
	void add_message(const EmailMessage &message)
	{
		messages_.push_back(message);
	}

	void connect() override
	{
		connected_ = true;
	}

	void disconnect() override
	{
		connected_ = false;
	}

	std::vector<EmailMessage> fetch_messages(const std::string &search_expression) override
	{
		(void)search_expression;
		return messages_;
	}

private:
	bool connected_{false};
	std::vector<EmailMessage> messages_;
};
