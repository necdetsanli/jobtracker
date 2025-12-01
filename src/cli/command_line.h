#pragma once

#include <string>
#include <vector>

/**
 * @brief Represents the supported command-line actions of the CLI.
 */
enum class CommandType
{
	None,
	List,
	Add,
	ImportCsv,
	ImportRemoteCsv,
	ImportImap
};

/**
 * @brief Parsed representation of the command-line arguments.
 */
struct CommandLineOptions
{
	/// Selected command to execute.
	CommandType command = CommandType::None;

	/// Optional path to a local CSV file.
	std::string csv_path;

	/// Optional remote CSV URL.
	std::string csv_url;

	/// Optional path to an IMAP configuration file.
	std::string imap_config_path;

	/// Additional free-form arguments.
	std::vector<std::string> extra_args;
};

/**
 * @brief Parse raw argc/argv into structured command-line options.
 *
 * @param argc Argument count as passed from main.
 * @param argv Argument values as passed from main.
 * @return Parsed CommandLineOptions.
 */
CommandLineOptions parse_arguments(int argc, char **argv);
