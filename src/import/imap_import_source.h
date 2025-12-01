#pragma once

#include <memory>
#include <string>
#include <vector>

#include "import/import_source.h"
#include "import/email_client.h"
#include "core/application.h"

/**
 * @brief Import source that pulls applications from an IMAP mailbox.
 *
 * This class does not know anything about SQLite or JobTracker.
 * It only depends on IEmailClient and translates email messages into Application objects.
 */
class ImapImportSource : public IImportSource
{
public:
	struct Config
	{
		std::string search_expression; ///< IMAP search query (e.g. "UNSEEN", "FROM ...", etc.)
		// Future: mapping rules (e.g. regex to extract company/position from subject)
	};

	ImapImportSource(std::unique_ptr<IEmailClient> client, Config config);

	std::vector<Application> fetch_applications() override;

private:
	std::unique_ptr<IEmailClient> client_;
	Config config_;

	Application map_email_to_application(const EmailMessage &message) const;
};
