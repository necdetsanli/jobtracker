#pragma once

#include <string>

/// Represents a single job application entry in the system.
struct Application
{
	/// Numeric primary key. Zero means "not persisted yet".
	int id = 0;

	/// Company name (e.g. "ACME Corp").
	std::string company;

	/// Position title (e.g. "C++ Developer").
	std::string position;

	/// Location of the job (e.g. "Remote", "Berlin, DE").
	std::string location;

	/// Source of the application (e.g. "linkedin", "email", "referral").
	std::string source;

	/// Current status (e.g. "applied", "screening", "interview", "offer", "rejected").
	std::string status;

	/// Date when you applied for the job (YYYY-MM-DD).
	std::string applied_date;

	/// Date of the last update for this application (YYYY-MM-DD).
	std::string last_update;

	/// Free-form notes (interview feedback, next steps, recruiter info, etc.).
	std::string notes;
};
