#pragma once
#include <string>
#include "UID.h"
#include <vector>
#include <imgui.h> // For ImVec2

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
		virtual ~Machine();
		LUINT::UID uid;

		/// Decorations ///
		std::string name;
		std::string manufacturer;
		virtual std::string get_description() { return std::string("Generic LUINT-architectured machine"); }
		std::vector<Machine*> connections;
		LUINT::Data::SessionData* session;
		ImVec2 get_window_pos() { return windowPos; }
		ImVec2 get_window_size() { return windowSize; }

		/// Actual important stuff ///
		// Pushes the machine's functions in a table to a Lua stack.
		virtual void PushFunctionsToStack(lua_State* state) {}

		// Mounts the machine data to the current active object in a Lua stack.
		void MountToCurrentTable(lua_State* state);

		// Unmounts the machine data from the current active object in a Lua stack.
		void UnmountFromCurrentTable(lua_State* state);

		// Renders this machine as a window in the GLFW window.
		void Render();

		virtual void OnConnect(Machine& other) {}
		virtual void OnDisconnect(Machine& other) {}

	protected:
		virtual void RenderChildWindows() {};
		virtual void RenderWindow();
		virtual void RenderMenuItems() {}

		inline virtual std::string GetWindowName()
		{
			return name;
		}

	private:
		void AddDefaultMenuItems();
		void ShowMachineInfo();

		bool showMachineInfo = false;
		bool editingName = false;

		ImVec2 windowPos;
		ImVec2 windowSize;
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
		void RenderChildWindows() override;
		void RenderMenuItems() override;

		void RenderTerminal();

	private:
		bool showStateInspector = false;
		bool showTerminal = false;
		const int terminalBufferSize = 128;
		char terminalBuffer[128] = "";
		std::vector<std::string> terminalLog;
	};

	struct Monitor : public Machine
	{
		Monitor(LUINT::Data::SessionData& _session, std::string _name, std::string _manufacturer);

		std::string get_description() override { return std::string("Shows data from a processing unit."); }
	};
}