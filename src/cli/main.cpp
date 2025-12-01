/// \file
/// \brief Entry point for the jobtracker CLI application.

#include "cli/command_line.h"
#include "core/application.h"
#include "core/job_tracker.h"
#include "storage/sqlite_application_repository.h"
#include "import/csv_import_source.h"
#include "import/remote_csv_import_source.h"
#include "import/import_service.h"
#include "import/http_client.h"

#include <exception>
#include <iostream>

/**
 * @brief Print a short usage message to stdout.
 */
static void print_usage()
{
	std::cout
		<< "jobtracker CLI\n\n"
		<< "Usage:\n"
		<< "  jobtracker_cli <command> [options]\n\n"
		<< "Commands:\n"
		<< "  help                   Show this help message\n"
		<< "  list                   List all applications\n"
		<< "  stats                  Show aggregated statistics\n"
		<< "  add                    Add a single application from flags\n"
		<< "  import-csv             Import applications from a local CSV file\n"
		<< "  import-remote-csv      Import applications from a remote CSV URL\n"
		<< "  import-imap            Import applications from an IMAP mailbox (not implemented yet)\n\n"
		<< "Common options:\n"
		<< "  --database <path>      Path to SQLite database file (required for most commands)\n"
		<< "  --csv <path>           Path to local CSV file (import-csv)\n"
		<< "  --remote-csv-url <url> Remote CSV URL (import-remote-csv)\n"
		<< "  --imap-config <path>   Path to IMAP config file (import-imap)\n"
		<< "  --company <name>       Company name (add)\n"
		<< "  --position <title>     Position title (add)\n"
		<< "  --location <location>  Job location (add)\n"
		<< "  --source <source>      Source of application (add)\n"
		<< "  --status <status>      Application status (add)\n"
		<< "  --notes <text>         Free-form notes (add)\n";
}

/**
 * @brief Entry point.
 */
int main(int argc, char **argv)
{
	try
	{
		CommandLineOptions options = parse_arguments(argc, argv);

		// Help or no command: just print usage and exit.
		if (options.command == CommandType::Help || options.command == CommandType::None)
		{
			print_usage();
			return 0;
		}

		if (options.command == CommandType::Unknown)
		{
			std::cerr << "Unknown command. Use 'help' to see available commands.\n";
			return 1;
		}

		// Commands that require a database.
		const bool needs_database =
			options.command == CommandType::List ||
			options.command == CommandType::Stats ||
			options.command == CommandType::Add ||
			options.command == CommandType::ImportCsv ||
			options.command == CommandType::ImportRemoteCsv ||
			options.command == CommandType::ImportImap;

		if (needs_database && options.database_path.empty())
		{
			std::cerr << "Database path is required. Use --database <file>.\n";
			return 1;
		}

		// For commands that touch the database, construct repository + tracker.
		SqliteApplicationRepository repository(options.database_path);
		JobTracker tracker(repository);

		switch (options.command)
		{
			case CommandType::List:
			{
				const auto apps = tracker.list_all();

				if (apps.empty())
				{
					std::cout << "No applications found.\n";
					return 0;
				}

				for (const auto &app : apps)
				{
					std::cout
						<< "[" << app.id << "] "
						<< app.company << " - " << app.position
						<< " (" << app.status << ")\n";
				}

				return 0;
			}

			case CommandType::Stats:
			{
				const Statistics stats = tracker.compute_statistics();

				if (stats.count_by_status.empty())
				{
					std::cout << "No applications found.\n";
					return 0;
				}

				std::cout << "Application statistics by status:\n";
				for (const auto &entry : stats.count_by_status)
				{
					std::cout << "  " << entry.first << ": " << entry.second << "\n";
				}

				return 0;
			}

			case CommandType::Add:
			{
				Application app{};
				app.company = options.company;
				app.position = options.position;
				app.location = options.location;
				app.source = options.source;
				app.status = options.status;
				app.notes = options.notes;

				Application saved = tracker.add(app);

				std::cout << "Added application with id " << saved.id << ".\n";
				return 0;
			}

			case CommandType::ImportCsv:
			{
				if (options.csv_path.empty())
				{
					std::cerr << "CSV path is required for import-csv. Use --csv <path>.\n";
					return 1;
				}

				CsvImportSource source(options.csv_path);
				ImportService service(source, repository);

				const ImportResult result = service.run_once();

				if (result.total == 0)
				{
					std::cout << "No applications found in CSV file.\n";
				}
				else if (result.imported == 0)
				{
					std::cout << "CSV contained " << result.total
						<< " rows, but none could be imported.\n";
				}
				else
				{
					std::cout << "Imported " << result.imported << " of "
						<< result.total << " applications from CSV.\n";
				}

				return 0;
			}

			case CommandType::ImportRemoteCsv:
			{
				if (options.remote_csv_url.empty())
				{
					std::cerr << "Remote CSV URL is required for import-remote-csv. "
						<< "Use --remote-csv-url <url>.\n";
					return 1;
				}

				LibcurlHttpClient http_client;

				RemoteCsvConfig config{};
				config.url = options.remote_csv_url;
				config.delimiter = ',';

				RemoteCsvImportSource source(http_client, config);
				ImportService service(source, repository);

				const ImportResult result = service.run_once();

				if (result.total == 0)
				{
					std::cout << "No applications found in remote CSV.\n";
				}
				else if (result.imported == 0)
				{
					std::cout << "Remote CSV contained " << result.total
						<< " rows, but none could be imported.\n";
				}
				else
				{
					std::cout << "Imported " << result.imported << " of "
						<< result.total << " applications from remote CSV.\n";
				}

				return 0;
			}

			case CommandType::ImportImap:
			{
				std::cerr << "IMAP import is not implemented yet.\n";
				return 1;
			}

			case CommandType::Help:
			case CommandType::None:
			case CommandType::Unknown:
			default:
				// Already handled above, but keep a defensive default.
				print_usage();
				return 1;
		}
	}
	catch (const std::exception &ex)
	{
		std::cerr << "Fatal error: " << ex.what() << "\n";
		return 1;
	}
	catch (...)
	{
		std::cerr << "Unknown fatal error.\n";
		return 1;
	}
}
