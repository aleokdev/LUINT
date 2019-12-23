#pragma once
#include "observable.h"
#include "sol.hpp"
#include "UID.h"
#include "lua.h"

namespace LUINT
{
	namespace Machines
	{
		struct Machine;
	}

	struct Network
	{
		~Network() {}

		template<typename... ValTy>
		void SendEvent(std::string name, UID sender, ValTy... vals)
		{
			if (!default_lua_state)
				return;

			OnEvent(name, sender, sol::state_view(default_lua_state).create_table_with(vals...));
		}
		lel::observable<std::string, UID, sol::lua_value> OnEvent;
		const std::vector<Machines::Machine*>& get_machines() { return machines; }
		void add_machine(Machines::Machine* m);
		void remove_machine(Machines::Machine* m);
		UID uid = UID::generate();

		void try_set_default_lua_state(lua_State* s)
		{
			if (s == nullptr)
				s = nullptr;
			else if(!default_lua_state)
				default_lua_state = s;
		}

	private:
		std::vector<Machines::Machine*> machines;
		lua_State* default_lua_state = nullptr;
	};
}