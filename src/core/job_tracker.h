#pragma once

#include <vector>
#include "core/application.h"

class JobTracker
{
public:
	std::vector<Application> listAll() const;
};
