#pragma once
#include <string>
#include "UID.h"
#include <vector>

struct lua_State;
struct ImGuiIO;
namespace LUINT::Data
{
	struct SessionData;
}
typedef int ImGuiWindowFlags;

#define MAX_MACHINENAME_LENGTH 32

namespace LUINT::Machines
{
	struct Machine
	{
		Machine(LUINT::Data::SessionData& _session, std::string _name, std::string _manufacturer);
		~Machine();
		LUINT::UID uid;

		/// Decorations ///
		std::string name;
		std::string manufacturer;
		virtual std::string get_description() { return std::string("Generic LUINT-architectured machine"); }
		std::vector<Machine*> connections;
		LUINT::Data::SessionData* session;

		/// Actual important stuff ///
		// Pushes the machine's functions in a table to a Lua stack.
		virtual void PushFunctionsToStack(lua_State* state) {}

		// Mounts the machine data to the current active object in a Lua stack.
		void MountToCurrentTable(lua_State* state);

		// Unmounts the machine data from the current active object in a Lua stack.
		void UnmountFromCurrentTable(lua_State* state);

		// Renders this machine as a window in the GLFW window.
		void Render();

	protected:
		virtual void RenderWindow();
		virtual void RenderMenuItems() {}

	private:
		void AddAboutMenuItem();
		bool BeginWindow(ImGuiWindowFlags flags = 0);

		bool showMachineInfo = false;
		bool editingName = false;
	};

	struct StateMachine : public Machine
	{
		StateMachine(LUINT::Data::SessionData& _session, std::string _name, std::string _manufacturer);

		inline lua_State* get_state() { return state; }

	protected:
		lua_State* state;
	};

	struct ProcessingUnit : public StateMachine
	{
		ProcessingUnit(LUINT::Data::SessionData& _session, std::string _name, std::string _manufacturer);

		std::string get_description() override { return std::string("Controllable machine that accepts input and can process user-given commands."); }

	protected:
		void RenderWindow() override;
		void RenderMenuItems() override;

	private:
		bool showStateInspector = false;
	};

	struct Monitor : public Machine
	{
		Monitor(LUINT::Data::SessionData& _session, std::string _name, std::string _manufacturer);

		std::string get_description() override { return std::string("Shows data from a processing unit."); }
	};

	struct Terminal : public Machine
	{
		Terminal(LUINT::Data::SessionData& _session, std::string _name, std::string _manufacturer);

		std::string get_description() override { return std::string("Can remotely connect to a processing unit to access it without an Operating System."); }
	};
}