/// \file
/// \brief Command-line argument parsing for the jobtracker CLI.

#include "cli/command_line.h"

#include <string>

CommandLineOptions parse_arguments(int argc, char **argv)
{
	CommandLineOptions options;

	// No command provided: show help by default.
	if (argc <= 1)
	{
		options.command = CommandType::Help;
		return options;
	}

	std::string command = argv[1];

	if (command == "list")
	{
		options.command = CommandType::List;
	}
	else if (command == "add")
	{
		options.command = CommandType::Add;
	}
	else if (command == "stats")
	{
		options.command = CommandType::Stats;
	}
	else if (command == "help" || command == "--help" || command == "-h")
	{
		options.command = CommandType::Help;
		return options;
	}
	else if (command == "import-csv")
	{
		options.command = CommandType::ImportCsv;
	}
	else if (command == "import-remote-csv")
	{
		options.command = CommandType::ImportRemoteCsv;
	}
	else if (command == "import-imap")
	{
		options.command = CommandType::ImportImap;
	}
	else
	{
		options.command = CommandType::Unknown;
		return options;
	}

	for (int i = 2; i < argc; ++i)
	{
		std::string arg = argv[i];

		auto require_value = [&](const char * /*name*/) -> const char *
		{
			if (i + 1 >= argc)
			{
				return nullptr;
			}
			++i;
			return argv[i];
		};

		if (arg == "--db" || arg == "--database")
		{
			const char *value = require_value("--database");
			if (value != nullptr)
			{
				options.database_path = value;
			}
		}
		else if (arg == "--csv")
		{
			const char *value = require_value("--csv");
			if (value != nullptr)
			{
				options.csv_path = value;
			}
		}
		else if (arg == "--remote-csv-url")
		{
			const char *value = require_value("--remote-csv-url");
			if (value != nullptr)
			{
				options.remote_csv_url = value;
			}
		}
		else if (arg == "--imap-config")
		{
			const char *value = require_value("--imap-config");
			if (value != nullptr)
			{
				options.imap_config_path = value;
			}
		}
		else if (arg == "--company")
		{
			const char *value = require_value("--company");
			if (value != nullptr)
			{
				options.company = value;
			}
		}
		else if (arg == "--position")
		{
			const char *value = require_value("--position");
			if (value != nullptr)
			{
				options.position = value;
			}
		}
		else if (arg == "--location")
		{
			const char *value = require_value("--location");
			if (value != nullptr)
			{
				options.location = value;
			}
		}
		else if (arg == "--source")
		{
			const char *value = require_value("--source");
			if (value != nullptr)
			{
				options.source = value;
			}
		}
		else if (arg == "--status")
		{
			const char *value = require_value("--status");
			if (value != nullptr)
			{
				options.status = value;
			}
		}
		else if (arg == "--notes")
		{
			const char *value = require_value("--notes");
			if (value != nullptr)
			{
				options.notes = value;
			}
		}
		else
		{
			// Unknown or positional argument: keep it for potential future use.
			options.extra_args.push_back(arg);
		}
	}

	return options;
}
