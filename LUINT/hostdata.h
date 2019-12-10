#pragma once
#include <vector>
#include <memory>
#include <utility>
#include "machine.h"

namespace LUINT
{
	namespace Data
	{
		struct SessionData
		{
			using Machine = LUINT::Machines::Machine;
			std::vector<std::unique_ptr<Machine>> machines;
			std::vector<std::pair<Machine*, Machine*>> connections;

			Machine* connecting = nullptr;
		};
	}
}