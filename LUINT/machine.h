#pragma once
#include <string>
#include "UID.h"

struct lua_State;
struct ImGuiIO;
typedef int ImGuiWindowFlags;

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
		bool BeginWindow(ImGuiWindowFlags flags = 0);

	private:
		bool showMachineInfo = false;
		bool editingName = false;
	};

	struct StateMachine : public Machine
	{
		StateMachine(std::string _name, std::string _manufacturer);

		inline lua_State* get_state() { return state; }

	protected:
		lua_State* state;
	};

	struct ProcessingUnit : public StateMachine
	{
		ProcessingUnit(std::string _name, std::string _manufacturer);

		std::string get_description() override { return std::string("Controllable machine that accepts input and can process user-given commands."); }

		virtual void Render() override;

	private:
		bool showStateInspector = false;
	};

	struct Monitor : public Machine
	{
		Monitor(std::string _name, std::string _manufacturer);

		std::string get_description() override { return std::string("Shows data from a processing unit."); }
	};

	struct Terminal : public Machine
	{
		Terminal(std::string _name, std::string _manufacturer);

		std::string get_description() override { return std::string("Can remotely connect to a processing unit to access it without an Operating System."); }
	};
}