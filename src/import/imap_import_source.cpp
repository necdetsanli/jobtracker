#include "import/imap_import_source.h"

#include "util/string_utils.h" // If you want to reuse trimming, etc.
#include "util/date_time.h"    // For default dates, if needed

ImapImportSource::ImapImportSource(std::unique_ptr<IEmailClient> client, Config config)
	: client_(std::move(client))
	, config_(std::move(config))
{
}

std::vector<Application> ImapImportSource::fetch_applications()
{
	std::vector<Application> result;

	client_->connect();
	const auto messages = client_->fetch_messages(config_.search_expression);
	client_->disconnect();

	result.reserve(messages.size());
	for (const auto &message : messages)
	{
		result.push_back(map_email_to_application(message));
	}

	return result;
}

Application ImapImportSource::map_email_to_application(const EmailMessage &message) const
{
	Application app;

	// Very naive mapping for now, this can be improved later.
	app.company = "";   // TODO: derive from subject or body
	app.position = "";  // TODO: derive from subject or body
	app.location = "";
	app.source = "email";
	app.status = "applied";

	app.notes = "Imported from email: " + message.subject;

	// Dates: you can convert from message.date, or reuse datetime::today_iso()
	app.applied_date = datetime::today_iso();
	app.last_update = datetime::today_iso();

	return app;
}
