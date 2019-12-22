#pragma once
#include <string>
#include "UID.h"
#include <vector>
#include <imgui.h> // For ImVec2
#include "machine_list.h"
#include "luainterface.h"
#include "sol.hpp"

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
		Machine(LUINT::Data::SessionData& _session, std::string _name);
		virtual ~Machine();
		LUINT::UID uid;

		/// Decorations ///
		std::string name;

		/// Actual important stuff ///
		LUINT::Data::SessionData* session;
		ImVec2 get_window_pos() { return windowPos; }
		ImVec2 get_window_size() { return windowSize; }

		// Renders this machine as a window in the GLFW window.
		void Render();

		virtual void OnConnect(Machine& other) {}
		virtual void OnDisconnect(Machine& other) {}

		virtual void ImplementLua(lua_State* state, sol::table&& proxy_table) {};

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

	private:
		void AddDefaultMenuItems();
		void ShowMachineInfo();

		bool showMachineInfo = false;
		bool editingName = false;
		bool showCannotConnectToItselfTooltip = false;

		ImVec2 windowPos;
		ImVec2 windowSize;
	};

	struct StateMachine : public Machine
	{
		StateMachine(LUINT::Data::SessionData& _session, std::string _name);

		inline lua_State* get_state() { return state; }

		virtual void Startup() = 0;
		virtual void Shutdown() = 0;

		virtual void PushEvent(std::string name, sol::table&& parameters) = 0;

		GENERATE_MACHINEINFO(StateMachine, (MachineInfo{ "State Machine", "aleok studios", "Generic machine that contains a Lua State." }));

	protected:
		lua_State* state;
	};

	struct ProcessingUnit : public StateMachine
	{
		ProcessingUnit(LUINT::Data::SessionData& _session, std::string _name);

		void Startup() override;
		void Shutdown() override {};

		void OnConnect(Machine& other) override;
		void OnDisconnect(Machine& other) override;

		void PushEvent(std::string name, sol::table&& parameters) override;

		GENERATE_MACHINEINFO(ProcessingUnit, (MachineInfo{ "Processing Unit", "aleok studios", "Controllable machine that accepts input and can process user-given commands.", Interfaces::get_LUINTProcessor() }));
		
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
		int ticks = 0;
		sol::basic_table_core<true, sol::reference>* impl_table;
		sol::coroutine main_coroutine;

		inline int f_ticks()
		{
			return ticks;
		};
	};

	struct Monitor : public Machine
	{
		Monitor(LUINT::Data::SessionData& _session, std::string _name);

		GENERATE_MACHINEINFO(Monitor, (MachineInfo{ "Monitor", "aleok studios", "Shows data from a processing unit." }));
	};

	struct Keyboard : public Machine
	{
		Keyboard(LUINT::Data::SessionData& _session, std::string _name) : Machine(_session, _name) {};

		GENERATE_MACHINEINFO(Keyboard, (MachineInfo{ "Keyboard", "aleok studios", "Simple machine that sends key_pressed and key_released events to state machines connected.", Interfaces::get_SimpleOutputDevice() }));

	protected:
		void RenderWindow() override;
	};

	struct LED : public Machine
	{
		LED(LUINT::Data::SessionData& _session, std::string _name) : Machine(_session, _name) {};

		GENERATE_MACHINEINFO(LED, (MachineInfo{ "LED", "aleok studios", "Simple machine that can turn on or off.", Interfaces::get_SimpleOutputDevice() }));

		void ImplementLua(lua_State* state, sol::table&& proxy_table) override;

		struct lua_impl
		{
			void do_thing()
			{
				std::cout << "thing";
			}
		};

		inline void f_set_state(bool new_state)
		{
			turnedOn = new_state;
		}

		inline bool f_get_state()
		{
			return turnedOn;
		}

	protected:
		void RenderWindow() override;

	private:
		bool turnedOn = false;
	};

#define MACHINE_TYPES _n(ProcessingUnit), _n(Monitor), _n(LED), _n(Keyboard)
	inline LUINT::Machines::List::MachineList<MACHINE_TYPES> allMachineTypes;
}