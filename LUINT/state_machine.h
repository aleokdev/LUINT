#pragma once
#include "machine.h"

namespace LUINT::Machines
{
	struct StateMachine : public Machine
	{
		StateMachine(Data::SessionData& _session, std::string _name, Network* _network) : Machine(_session, _name, _network)
		{
			state = luaL_newstate();
		}

		inline lua_State* get_state() { return state; }

		virtual void Startup() = 0;
		virtual void Shutdown() = 0;

		virtual void PushEvent(std::string name, UID sender, sol::lua_value parameters) = 0;

		GENERATE_MACHINEINFO(StateMachine, (MachineInfo{ "State Machine", "aleok studios", "Generic machine that contains a Lua State." }));

	protected:
		lua_State* state;
	};
}