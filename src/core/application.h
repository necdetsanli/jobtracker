#pragma once

#include <string>

/**
 * @brief Represents a single job application entry in the tracker.
 */
struct Application
{
	/// Unique identifier assigned by the storage layer (e.g. SQLite autoincrement).
	int id = 0;

	/// Company name of the job application.
	std::string company;

	/// Position or role title applied for.
	std::string position;

	/// Location of the job (city, country, or "Remote").
	std::string location;

	/// Current status of the application (e.g. "applied", "interview", "offer").
	std::string status;

	/// Date when the application was submitted (ISO format: YYYY-MM-DD).
	std::string applied_date;

	/// Date of the last status update (ISO format: YYYY-MM-DD).
	std::string last_update;

	/// Source of the application (e.g. "linkedin", "company_portal", "remote_csv").
	std::string source;

	/// Free-form notes about the application (recruiter name, interview details, etc.).
	std::string notes;
};
