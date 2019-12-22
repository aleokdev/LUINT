#pragma once
#include "observable.h"
#include "sol.hpp"

namespace LUINT
{
	struct Network
	{
		void SendEvent(std::string name, sol::table args);
		lel::observable<std::string, sol::table> OnEvent;
	};
}