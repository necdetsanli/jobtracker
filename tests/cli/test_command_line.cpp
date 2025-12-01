#include <catch2/catch_test_macros.hpp>

#include "cli/command_line.h"

TEST_CASE("parse_arguments_defaults_to_help_when_no_command_is_given")
{
	char *argv[] = {
		const_cast<char *>("jobtracker_cli")
	};
	int argc = 1;

	CommandLineOptions options = parse_arguments(argc, argv);

	REQUIRE(options.command == CommandType::Help);
}

TEST_CASE("parse_arguments_parses_list_command_and_db_path")
{
	char *argv[] = {
		const_cast<char *>("jobtracker_cli"),
		const_cast<char *>("list"),
		const_cast<char *>("--db"),
		const_cast<char *>("test.db")
	};
	int argc = 4;

	CommandLineOptions options = parse_arguments(argc, argv);

	REQUIRE(options.command == CommandType::List);
	REQUIRE(options.database_path == "test.db");
}

TEST_CASE("parse_arguments_parses_add_command_with_required_fields")
{
	char *argv[] = {
		const_cast<char *>("jobtracker_cli"),
		const_cast<char *>("add"),
		const_cast<char *>("--company"),
		const_cast<char *>("ACME"),
		const_cast<char *>("--position"),
		const_cast<char *>("C++ Developer"),
		const_cast<char *>("--status"),
		const_cast<char *>("applied")
	};
	int argc = 8;

	CommandLineOptions options = parse_arguments(argc, argv);

	REQUIRE(options.command == CommandType::Add);
	REQUIRE(options.company == "ACME");
	REQUIRE(options.position == "C++ Developer");
	REQUIRE(options.status == "applied");
}
