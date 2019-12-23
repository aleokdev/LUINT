#pragma once
#include "machine.h"
#include "state_machine.h"

namespace LUINT::Machines
{
	struct ProcessingUnit : public StateMachine
	{
		ProcessingUnit(LUINT::Data::SessionData& _session, std::string _name, Network* _network);

		// Call before Startup(). Setups functions, tables, etc.
		void Setup();
		void Startup() override;
		void Shutdown() override;
		void Reboot();

		void OnConnect(Machine& other) override;
		void OnDisconnect(Machine& other) override;

		void PushEvent(std::string name, UID sender, sol::lua_value parameters) override;

		GENERATE_MACHINEINFO(ProcessingUnit, (MachineInfo{ "Processing Unit", "aleok studios", "Controllable machine that accepts input and can process user-given commands.", Interfaces::get_LUINTProcessor() }));

	protected:
		void RenderWindow() override;
		void RenderChildWindows() override;
		void RenderMenuItems() override;

		void RenderTerminal();

		int f_ticks() { return ticks_since_startup; }

	private:
		bool showStateInspector = false;
		bool showTerminal = false;
		const int terminalBufferSize = 128;
		char terminalBuffer[128] = "";
		std::vector<std::string> terminalLog;
		sol::basic_table_core<true, sol::reference>* impl_table;
		sol::coroutine main_coroutine;
		bool is_on = false;
		// How often is the "tick" event sent to the processor, approximately.
		int ticks_per_second = 20;
		float time_since_last_tick = 0.f;
		int ticks_since_startup = 0;

	};
}