#pragma once
#include <vector>
#include <memory>
#include "machine.h"

namespace LUINT
{
	namespace Data
	{
		inline std::vector<std::unique_ptr<LUINT::Machines::Machine>> machines;
	}
}