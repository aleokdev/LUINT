#pragma once
#include <vector>
#include <memory>
#include <utility>
#include "machine.h"
#include "network.h"

namespace LUINT
{
	namespace Data
	{
		struct SessionData
		{
			using Machine = LUINT::Machines::Machine;
			std::vector<std::unique_ptr<Machine>> machines;
			std::vector<Network> networks;

			Network* connecting = nullptr;
		};
	}
}