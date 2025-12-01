#pragma once

#include <string>

/// Possible top-level commands for the CLI.
enum class CommandType
{
	List,
	Add,
	Stats,
	Help,
	Unknown
};

/// Parsed representation of command-line arguments for jobtracker_cli.
///
/// The same structure is used both in the real CLI and in unit tests.
struct CommandLineOptions
{
	/// Selected command parsed from argv.
	CommandType command = CommandType::Help;

	/// Path to the SQLite database file (default: data/jobtracker.db).
	std::string database_path = "data/jobtracker.db";

	// Fields for the "add" command:
	/// Company name for a new application.
	std::string company;

	/// Position title for a new application.
	std::string position;

	/// Job location (optional).
	std::string location;

	/// Source of the application (e.g. "linkedin", "email").
	std::string source;

	/// Initial status (optional, defaults to "applied" at domain level).
	std::string status;

	/// Notes for the application (optional).
	std::string notes;
};

/// Parses command-line arguments into a CommandLineOptions structure.
///
/// This function does not perform any I/O and is therefore easy to unit test.
///
/// \param argc Argument count.
/// \param argv Argument values.
/// \return Parsed command-line options.
CommandLineOptions parse_arguments(int argc, char **argv);
