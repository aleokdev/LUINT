#pragma once
#include <vector>
#include <memory>
#include <utility>
#include "api/machine.hpp"
#include "api/network.hpp"

namespace LUINT
{
	namespace Data
	{
		struct SessionData
		{
			using Machine = LUINT::Machines::Machine;
			std::vector<std::unique_ptr<Machine>> machines;
			std::vector<std::unique_ptr<Network>> networks;

			Network* connecting = nullptr;
		};
	}
}