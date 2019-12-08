#pragma once
#include <string>
#include "UID.h"

struct lua_State;
struct ImGuiIO;

#define MAX_MACHINENAME_LENGTH 32

namespace LUINT::Machines
{
	struct Machine
	{
		Machine(std::string _name, std::string _manufacturer);
		LUINT::UID uid;

		/// Decorations ///
		std::string name;
		std::string manufacturer;
		virtual std::string get_description() { return std::string("Generic LUINT-architectured machine"); }

		/// Actual important stuff ///
		// Pushes the machine's functions in a table to a Lua stack.
		virtual void PushFunctionsToStack(lua_State* state) {}

		// Mounts the machine data to the current active object in a Lua stack.
		void MountToCurrentTable(lua_State* state);

		// Unmounts the machine data from the current active object in a Lua stack.
		void UnmountFromCurrentTable(lua_State* state);

		// Renders this machine as a window in the GLFW window.
		virtual void Render();

	protected:
		void AddAboutMenuItem();
	};

	struct StateMachine : public Machine
	{
		StateMachine(std::string _name, std::string _manufacturer);

		inline lua_State* get_state() { return state; }

	protected:
		lua_State* state;
	};

	struct Computer : public StateMachine
	{
		Computer(std::string _name, std::string _manufacturer);

		std::string get_description() override { return std::string("Controllable machine that accepts input and can process user-given commands."); }

		virtual void Render() override;
	};

	struct Terminal : public Machine
	{
		Terminal(std::string _name, std::string _manufacturer);

		std::string get_description() override { return std::string("Controllable machine that accepts input and can process user-given commands."); }
	};
}