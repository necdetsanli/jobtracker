#pragma once

#include <string>
#include <vector>

/**
 * @brief Supported CLI commands.
 */
enum class CommandType
{
	None,
	Help,
	List,
	Stats,
	Add,
	ImportCsv,
	ImportRemoteCsv,
	ImportImap,
	Unknown
};

/**
 * @brief Parsed command-line options for the jobtracker CLI.
 */
struct CommandLineOptions
{
	/// Selected command to execute.
	CommandType command = CommandType::None;

	/// Path to the SQLite database file.
	std::string database_path;

	/// Optional path to a local CSV file for import.
	std::string csv_path;

	/// Optional remote CSV URL.
	std::string remote_csv_url;

	/// Optional path to an IMAP configuration file.
	std::string imap_config_path;

	/// Optional company name for add/update commands.
	std::string company;

	/// Optional position title for add/update commands.
	std::string position;

	/// Optional job location.
	std::string location;

	/// Optional application source (e.g. "linkedin").
	std::string source;

	/// Optional status (e.g. "applied").
	std::string status;

	/// Optional free-form notes.
	std::string notes;

	/// Additional free-form arguments after known flags.
	std::vector<std::string> extra_args;
};

/**
 * @brief Parse command-line arguments into a CommandLineOptions structure.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Parsed CommandLineOptions structure.
 */
CommandLineOptions parse_arguments(int argc, char **argv);
