#pragma once
#include <vector>

namespace LUINT::Machines
{
	struct LuaInterfaceFunction
	{
		const char* name;
		const char* description;
		const struct Argument
		{
			const char* name;
		};
		std::vector<Argument> arguments;
	};

	struct LuaInterface
	{
		LuaInterface()
		{
			name = "empty";
			description = "empty";
		}

		const char* name;
		const char* description;
		std::vector<LuaInterfaceFunction> functions;
	};

	namespace Interfaces
	{
		inline LuaInterface get_LUINTProcessor()
		{
			LuaInterface result;

			result.name = "LUINT Processor Architecture";
			result.description = "Contains a set of functions that generic LUINT processors possess. These are available to the local "
									"processor inside the global table [computer] at startup, and also are available to other "
									"connected machines via [computer.connections].";
			result.functions = std::vector<LuaInterfaceFunction>
			{
				{
					"ticks",
					"Returns the number of ticks that have passed since the startup of the machine."
				}
			};

			return result;
		}
	}
}