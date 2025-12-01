#include <catch2/catch_test_macros.hpp>

#include "import/imap_import_source.h"
#include "import/email_message.h"
#include "tests/import/fake_email_client.h"

TEST_CASE("ImapImportSource_converts_emails_to_applications")
{
	auto fake_client = std::make_unique<FakeEmailClient>();

	EmailMessage msg;
	msg.id = "1";
	msg.from = "hr@example.com";
	msg.to = "me@example.com";
	msg.subject = "Application received for C++ Developer at ACME";
	// msg.body_text = "...";
	// msg.date = ...; // can be default for now

	fake_client->add_message(msg);

	ImapImportSource::Config config;
	config.search_expression = "ALL";

	ImapImportSource source(std::move(fake_client), config);

	const auto apps = source.fetch_applications();

	REQUIRE(apps.size() == 1);
	REQUIRE(apps[0].source == "email");
	REQUIRE_FALSE(apps[0].notes.empty());
}
