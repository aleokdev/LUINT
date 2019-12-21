#pragma once
#include <vector>
#include <unordered_map>

struct lua_State;
typedef int(*lua_CFunction) (lua_State *L);

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

		inline LuaInterface get_SimpleOutputDevice()
		{
			LuaInterface result;

			result.name = "Simple Output Device";
			result.description = "Contains two simple functions: One for setting the state of the machine and "
				"another one for getting the state that was set before. Used for simple machines such as LEDs.";
			result.functions = std::vector<LuaInterfaceFunction>
			{
				{
					"set_state",
					"Sets the state of the machine. The value type used is normally obvious: For example, if the machine "
					"being controlled is a LED, it'll be a boolean, and if it is a digit display, it'll be an integer.",
					"nil",
					std::vector<LuaInterfaceFunction::Argument> { {"state"} }
				},
				{
					"get_state",
					"Gets the current state of the machine. If not previously set with set_state, will return the default "
					"state. The type returned is the same used in set_state.",
					"state"
				}
			};

			return result;
		}
	}
}