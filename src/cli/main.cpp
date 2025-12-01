/// \file
/// \brief Entry point for the jobtracker CLI application.

#include <exception>
#include <iostream>
#include <filesystem>


#include "cli/command_line.h"
#include "core/job_tracker.h"
#include "storage/sqlite_application_repository.h"
#include "import/csv_import_source.h"
#include "import/import_service.h"


static void print_help()
{
	std::cout
		<< "Usage: jobtracker_cli <command> [options]\n"
		<< "\n"
		<< "Commands:\n"
		<< "  list                 List all applications\n"
		<< "  add                  Add a new application\n"
		<< "  stats                Show application statistics\n"
		<< "  import-csv           Import applications from a CSV file\n"
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
		<< "  --notes TEXT         Notes\n"
		<< "\n"
		<< "Options for 'import-csv':\n"
		<< "  --csv PATH           Path to CSV file with applications\n";
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

			if (apps.empty())
			{
				std::cout
					<< "No applications found.\n"
					<< "Use 'add' or 'import-csv' to create applications.\n";
				return 0;
			}

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

			if (stats.count_by_status.empty())
			{
				std::cout
					<< "No applications found.\n"
					<< "There are no statistics to display yet.\n";
				return 0;
			}

			std::cout << "Applications by status:\n";
			for (const auto &entry : stats.count_by_status)
			{
				std::cout << "  " << entry.first << ": " << entry.second << "\n";
			}

			return 0;
		}

		if (options.command == CommandType::ImportCsv)
		{
			if (options.csv_path.empty())
			{
				std::cerr << "Error: --csv PATH is required for 'import-csv'.\n";
				return 1;
			}

			if (!std::filesystem::exists(options.csv_path))
			{
				std::cerr
					<< "Error: CSV file '" << options.csv_path << "' does not exist.\n"
					<< "Please check the path and try again.\n";
				return 1;
			}

			auto repository = std::make_unique<SqliteApplicationRepository>(options.database_path);
			JobTracker tracker(std::move(repository));

			CsvImportSource source(options.csv_path);
			ImportService service(tracker, source);

			const ImportResult result = service.run_once();

			if (result.total == 0)
			{
				std::cout
					<< "No rows found in CSV file '" << options.csv_path << "'.\n"
					<< "Make sure the file is not empty and has a header row.\n";
				return 0;
			}

			if (result.imported == 0)
			{
				std::cout
					<< "No applications were imported from '" << options.csv_path << "'.\n"
					<< "Check that each row has at least a company or position.\n";
				return 0;
			}

			std::cout
				<< "Imported " << result.imported
				<< " applications out of " << result.total
				<< " rows from CSV.\n";

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
