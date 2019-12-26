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

	struct LuaInterfaceEvent
	{
		const char* name;
		const char* type;
		const char* description;
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
		std::vector<LuaInterfaceEvent> events_sent;
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
					"get_connection_interface",
					"Returns a table with information about a connected machine's interface.",
					"{ name = interface_name, description = interface_description }",
					std::vector<LuaInterfaceFunction::Argument> { {"UID"} }
				},
				{
					"shutdown",
					"Turns off the processor. Not recommended to use raw, since the machine will be turned off just after the current tick. It is recommended to wrap "
					"this function to push an event when it is called.",
					"nil"
				},
				{
					"reboot",
					"Turns off and on the processor. Not recommended to use raw, since the machine will be turned off just after the current tick. It is recommended to wrap "
					"this function to push an event when it is called.",
					"nil"
				},
				{
					"push",
					"Pushes an event to the event queue, that will be processed in FIFO order when the machine's coroutine is executed.",
					"nil",
					std::vector<LuaInterfaceFunction::Argument> { {"name"}, {"args..."} }
				},
				{
					"push",
					"Pushes an event to the event queue, that will be processed in FIFO order when the machine's coroutine is executed.",
					"nil",
					std::vector<LuaInterfaceFunction::Argument> { {"name"}, {"args..."} }
				},
				{
					"broadcast_packet",
					"Sends a packet (event) through the whole network.",
					"nil",
					std::vector<LuaInterfaceFunction::Argument> { {"packet_name"}, {"data..."} }
				},
				{
					"send_packet",
					"Sends a packet (event) to a specific machine with a given UID in the current network. Returns true if the machine's "
					"UID was found in the network (and thus the packet was sent correctly), and false if not.",
					"bool",
					std::vector<LuaInterfaceFunction::Argument> { {"packet_name"}, {"UID"}, {"data..."} }
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

		inline LuaInterface get_Keyboard()
		{
			LuaInterface result;

			result.name = "Generic Keyboard Input Device";
			result.description = "Sends two simple events: key_pressed and key_released.";
			result.events_sent = std::vector<LuaInterfaceEvent>
			{
				{
					"key_pressed",
					"[number, string]",
					"Sent when a key is pressed on the keyboard. Returns the keycode pressed as a number, and the corresponding"
					"character typed (nil if there's not a key assigned to it, like enter, escape or any function key)"
				},
				{
					"key_released",
					"[number, string]",
					"Sent when a key is released on the keyboard. Returns the keycode released as a number, and the corresponding"
					"character related to that keycode (nil if there's not a key assigned to it, like enter, escape or any function key)"
				}
			};

			return result;
		}

		inline LuaInterface get_Button()
		{
			LuaInterface result;

			result.name = "Generic Button Input Device";
			result.description = "Sends two simple events: button_pressed and button_released.";
			result.events_sent = std::vector<LuaInterfaceEvent>
			{
				{
					"button_pressed",
					"[]",
					"Sent when the button is pressed (clicked on)."
				},
				{
					"button_released",
					"[]",
					"Sent when the button is released."
				}
			};

			return result;
		}

		inline LuaInterface get_PTMMonitor()
		{
			LuaInterface result;

			result.name = "Persistent Text Monitor";
			result.description = "Contains functions that set or get characters from the screen.";
			result.functions = std::vector<LuaInterfaceFunction>
			{
				{
					"set",
					"Sets a character on the screen. The coordinates go from {1,1} to {max_res.width, max_res.height}.\n"
					"Sets only ONE character. If a string with multiple characters is given, only the first char will be set "
					"and the other ones will be ignored. If X or Y don't belong inside the screen resolution, the monitor will "
					"not do anything.",
					"nil",
					std::vector<LuaInterfaceFunction::Argument> { {"char"}, {"x"}, {"y"} }
				},
				{
					"fill",
					"Fills a given portion of the screen with a certain character. The coordinates go from {1,1} to {max_res.width, max_res.height}.\n"
					"Sets only ONE character. If a string with multiple characters is given, only the first char will be set "
					"and the other ones will be ignored.\n"
					"X and Y specify the position of the portion, and W and H specify the width and height, respectively. "
					"If X or Y don't belong inside the screen resolution, the monitor will "
					"not do anything.\n"
					"If X+W or Y+H don't belong inside the screen resolution, the behaviour will depend on the interface implementation.",
					"nil",
					std::vector<LuaInterfaceFunction::Argument> { {"char"}, {"x"}, {"y"}, {"w"}, {"h"} }
				}
			};

			return result;
		}
	}
}