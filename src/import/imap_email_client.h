#pragma once

#include <string>

#include "import/email_client.h"

/**
 * @brief IMAP-based email client.
 *
 * This is a placeholder for a real IMAP implementation. It can be backed by
 * any IMAP library (e.g. libcurl, mailio, etc.). For now, it can be left as
 * an unimplemented stub or a minimal prototype.
 */
class ImapEmailClient : public IEmailClient
{
public:
	struct ConnectionSettings
	{
		std::string host;
		int port = 993;
		bool use_tls = true;

		std::string username;
		std::string password; // In practice, use app passwords or environment variables.
		std::string mailbox = "INBOX"; // Or a Gmail label, e.g. "[Gmail]/All Mail".
	};

	explicit ImapEmailClient(ConnectionSettings settings);

	void connect() override;
	void disconnect() override;
	std::vector<EmailMessage> fetch_messages(const std::string &search_expression) override;

private:
	ConnectionSettings settings_;
	// Internal IMAP client state / handles will go here.
};
