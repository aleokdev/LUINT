#pragma once
#include "machine.h"
#include "state_machine.h"

namespace LUINT::Machines
{
	struct ProcessingUnit : public StateMachine
	{
		ProcessingUnit(LUINT::Data::SessionData& _session, std::string _name, Network* _network);

		void Startup() override;
		void Shutdown() override {};

		void OnConnect(Machine& other) override;
		void OnDisconnect(Machine& other) override;

		void PushEvent(std::string name, UID sender, sol::lua_value parameters) override;

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
		bool is_on = false;

		inline int f_ticks()
		{
			return ticks;
		};
	};
}