#pragma once
#include <string>
#include "UID.h"
#include <vector>
#include <imgui.h> // For ImVec2
#include "machine_list.tpp"
#include "luainterface.h"
#include "sol.hpp"
#include "network.h"

struct lua_State;
struct ImGuiIO;
namespace LUINT::Data
{
	struct SessionData;
}
typedef int ImGuiWindowFlags;

#define MAX_MACHINENAME_LENGTH 32

#define _n(c) LUINT::Machines::##c
#define GENERATE_MACHINEINFO(T, m)					\
inline static const _n(MachineInfo) static_info = m;	\
const _n(MachineInfo)& get_info() override { return _n(T)::static_info; }


namespace LUINT::Machines
{
	struct MachineInfo
	{
		const char* name;
		const char* manufacturer;
		const char* description;
		const LuaInterface interface;
	};

	struct Machine;

	struct Machine
	{
		Machine(LUINT::Data::SessionData& _session, std::string _name, Network* _network);
		virtual ~Machine();
		LUINT::UID uid;

		/// Decorations ///
		std::string name;

		/// Actual important stuff ///
		LUINT::Data::SessionData* session;
		Network* network = nullptr;
		ImVec2 get_window_pos() { return windowPos; }
		ImVec2 get_window_size() { return windowSize; }

		// Renders this machine as a window in the GLFW window.
		void Render();

		virtual void OnConnect(Machine& other) {}
		virtual void OnDisconnect(Machine& other) {}

		virtual void ImplementLua(lua_State* state, sol::table& proxy_table) {};

		// The MachineInfo of every machine acts as a "static unique identifier" for every machine.
		inline static const MachineInfo static_info = MachineInfo{ "Machine", "aleok studios", "You shouldn't be seeing this." };

		virtual const MachineInfo& get_info() = 0;

	protected:
		virtual void RenderChildWindows() {};
		virtual void RenderWindow();
		virtual void RenderMenuItems() {}

		inline virtual std::string GetWindowName()
		{
			return name;
		}

		virtual ImGuiWindowFlags GetWindowFlags() { return ImGuiWindowFlags_MenuBar; }

	private:
		void AddDefaultMenuItems();
		void ShowMachineInfo();

		bool showMachineInfo = false;
		bool editingName = false;
		bool showCannotConnectToItselfTooltip = false;

		ImVec2 windowPos;
		ImVec2 windowSize;
	};
}