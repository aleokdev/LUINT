#pragma once
#include "machine.h"
#include "lua.h"
#include <memory>
#include <queue>
#include <tuple>
#include <unordered_map>

namespace LUINT::Machines
{
	struct ProcessingUnit : public Machine
	{
		ProcessingUnit(LUINT::Data::SessionData& _session, std::string _name, Network* _network);

		// Call before Startup(). Setups functions, tables, etc.
		void Setup();
		void Startup();
		void Shutdown();
		void Reboot();

		void ReconnectAll();

		void OnConnect(Machine& other) override;
		void OnDisconnect(Machine& other) override;

		void PushEvent(std::string name, UID sender, sol::lua_value parameters);

		GENERATE_MACHINEINFO(ProcessingUnit, (MachineInfo{ "Processing Unit", "aleok studios", "Controllable machine that accepts input and can process user-given commands.", Interfaces::get_LUINTProcessor() }));

	protected:
		void RenderWindow() override;
		void RenderChildWindows() override;
		void RenderMenuItems() override;

		void RenderTerminal();

		void Tick();

		int f_ticks() { return ticks_since_startup; }
		sol::table GetNetworkUIDs();
		sol::table GetProxy(std::string);

	private:
		lua_State* state = nullptr;
		bool showStateInspector = false;
		bool showTerminal = false;
		const int terminalBufferSize = 128;
		char terminalBuffer[128] = "";
		std::vector<std::string> terminalLog;
		std::unordered_map<std::string, sol::table> proxies;
		std::queue<std::tuple<std::string, UID, sol::lua_value>> eventQueue;
		sol::basic_table_core<true, sol::reference>* impl_table;
		std::unique_ptr<sol::coroutine> main_coroutine;
		bool is_on = false;
		// How often is the "tick" event sent to the processor, approximately.
		int ticks_per_second = 20;
		float time_since_last_tick = 0.f;
		int ticks_since_startup = 0;

		enum class AfterTickAction
		{
			none,
			shutdown,
			reboot
		};
		AfterTickAction after_tick = AfterTickAction::none;

	};
}