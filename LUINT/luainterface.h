#pragma once
#include <vector>

namespace LUINT::Machines
{
	struct LuaInterfaceFunction
	{
		const char* name;
		const char* description;
		const char* returns;
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
					"Returns the number of ticks that have passed since the startup of the machine.",
					"ticks [int]"
				},
				{
					"get_info",
					"Returns a table with information about a connected machine or about the local processor.\n"
					"Run without arguments to get info about the local machine.",
					"info [table {"
						"name [string], description [string], interface [table {"
							"name [string], description [string], functions [table {name [string], description [string], args [table {name [string]}]}]"
					"} ]} ]",
					std::vector<LuaInterfaceFunction::Argument> { {"[UID]"} }
				},
				{
					"shutdown",
					"Turns off the processor. Not recommended to use raw, since the machine will be turned off inmediately. It is recommended to wrap "
					"this function to push an event when it is called.",
					"nil"
				},
				{
					"reboot",
					"Turns off and on the processor. Not recommended to use raw, since the machine will be turned off inmediately. It is recommended to wrap "
					"this function to push an event when it is called.",
					"nil"
				},
				{
					"push_signal",
					"Pushes a signal to the signal queue, that will be processed in FIFO order when pull_signal is called.",
					"nil",
					std::vector<LuaInterfaceFunction::Argument> { {"name"}, {"args..."} }
				},
				{
					"pull_signal",
					"Pulls a signal from the signal queue. Older signals will be processed first.",
					"signal_name, args..."
				}
			};

			return result;
		}
	}
}