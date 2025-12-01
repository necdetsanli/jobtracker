/// \file
/// \brief Entry point for the jobtracker CLI application.

#include <exception>
#include <iostream>

#include "cli/command_line.h"
#include "core/job_tracker.h"
#include "storage/sqlite_application_repository.h"

static void print_help()
{
	std::cout
		<< "Usage: jobtracker_cli <command> [options]\n"
		<< "\n"
		<< "Commands:\n"
		<< "  list                 List all applications\n"
		<< "  add                  Add a new application\n"
		<< "  stats                Show application statistics\n"
		<< "  help                 Show this help message\n"
		<< "\n"
		<< "Common options:\n"
		<< "  --db PATH            Path to SQLite database (default: data/jobtracker.db)\n"
		<< "\n"
		<< "Options for 'add':\n"
		<< "  --company NAME       Company name (required)\n"
		<< "  --position TITLE     Position title (required)\n"
		<< "  --location TEXT      Job location\n"
		<< "  --source TEXT        Source of the application\n"
		<< "  --status TEXT        Initial status (default: applied)\n"
		<< "  --notes TEXT         Notes\n";
}

int main(int argc, char **argv)
{
	try
	{
		CommandLineOptions options = parse_arguments(argc, argv);

		if (options.command == CommandType::Help)
		{
			print_help();
			return 0;
		}

		if (options.command == CommandType::Unknown)
		{
			std::cerr << "Unknown command. Use 'help' for usage information.\n";
			return 1;
		}

		auto repository = std::make_unique<SqliteApplicationRepository>(options.database_path);
		JobTracker tracker(std::move(repository));

		if (options.command == CommandType::List)
		{
			const auto apps = tracker.list_all();
			for (const auto &app : apps)
			{
				std::cout
					<< "#" << app.id << " | "
					<< app.company << " | "
					<< app.position << " | "
					<< app.status << " | "
					<< app.applied_date
					<< "\n";
			}
			return 0;
		}

		if (options.command == CommandType::Add)
		{
			if (options.company.empty() || options.position.empty())
			{
				std::cerr << "Error: --company and --position are required for 'add'.\n";
				return 1;
			}

			Application app;
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

		if (options.command == CommandType::Stats)
		{
			const Statistics stats = tracker.compute_statistics();
			std::cout << "Applications by status:\n";
			for (const auto &entry : stats.count_by_status)
			{
				std::cout << "  " << entry.first << ": " << entry.second << "\n";
			}
			return 0;
		}

		std::cerr << "Unhandled command.\n";
		return 1;
	}
	catch (const std::exception &ex)
	{
		std::cerr << "Fatal error: " << ex.what() << "\n";
		return 1;
	}
}
