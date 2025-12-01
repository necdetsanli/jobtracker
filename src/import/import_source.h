#pragma once

#include <vector>

#include "core/application.h"

/**
 * @brief Abstract interface for sources that can provide job applications.
 *
 * Implementations can read applications from CSV files, IMAP mailboxes,
 * remote HTTP endpoints, or any other source.
 */
class IImportSource
{
public:
	virtual ~IImportSource() = default;

	/**
	 * @brief Fetch applications from the underlying source.
	 *
	 * @return A vector of Application objects read from the source.
	 */
	virtual std::vector<Application> fetch_applications() = 0;
};
