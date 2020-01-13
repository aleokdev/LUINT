#pragma once
#include "api/machine.hpp"
#include "lua.h"
#include <memory>
#include <queue>
#include <tuple>
#include <unordered_map>
#include <functional>
#include "api/network.hpp"
#include "misc/biosedit_window.hpp"

namespace LUINT::Machines
{
	struct ProcessingUnit : public Machine
	{
		ProcessingUnit(LUINT::Data::SessionData& _session, std::string _name, Network* _network);
		~ProcessingUnit();

		// Call before Startup(). Setups functions, tables, etc.
		void Setup();
		void Startup();
		void Shutdown();
		void Reboot();

		void ReconnectAll();

		void OnConnect(Machine& other) override;
		void OnDisconnect(Machine& other) override;

		void PushEvent(Network::Event);

		GENERATE_MACHINEINFO(ProcessingUnit, (MachineInfo{ "Processing Unit", "aleok studios", "Controllable machine that accepts input and can process user-given commands.", Interfaces::get_LUINTProcessor() }));

	protected:
		void RenderWindow() override;
		void RenderChildWindows() override;
		void RenderMenuItems() override;
		virtual void OnChangeNetwork(Network* prev, Network* next) override;

		void RenderTerminal();

		void Tick();

		int f_ticks() { return ticks_since_startup; }
		sol::table GetNetworkUIDs();

	private:
		lua_State* state = nullptr;
		bool showStateInspector = false;
		bool showTerminal = false;
		const int terminalBufferSize = 128;
		char terminalBuffer[128] = "";
		std::vector<std::string> terminalLog;
		std::queue<Network::Event> eventQueue;
		std::unique_ptr<sol::coroutine> main_coroutine;
		bool is_on = false;
		// How often is the "tick" event sent to the processor, approximately.
		int ticks_per_second = 20;
		float time_since_last_tick = 0.f;
		int ticks_since_startup = 0;
		std::unique_ptr<GUI::BIOSEditWindow> bioseditwindow;
		// TODO: Replace #include with #embed_str, when MSVC supports it
		std::string bios =
			#include "bios/monitor_test_bios.lua"
		;

		enum class AfterTickAction
		{
			none,
			shutdown,
			reboot
		};
		AfterTickAction after_tick = AfterTickAction::none;

	};
}