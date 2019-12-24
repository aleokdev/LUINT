#include "network.h"
#include <algorithm>
#include "machine.h"

namespace LUINT
{
	bool Network::is_connected(Machines::Machine * m)
	{
		return std::count(machines.begin(), machines.end(), m);
	}
	void Network::add_machine(Machines::Machine* m)
	{
		for (auto& machine : machines)
		{
			machine->OnConnect(*m);
			m->OnConnect(*machine);
		}

		machines.emplace_back(m);
	}

	void Network::add_state(lua_State* s)
	{
		states.emplace_back(s);
	}

	bool Network::has_state(lua_State* s)
	{
		return std::count(states.begin(), states.end(), s);
	}

	void Network::remove_state(lua_State* s)
	{
		states.erase(std::remove(states.begin(), states.end(), s), states.end());
	}

	void Network::remove_machine(Machines::Machine * m)
	{
		for (auto& machine : machines)
		{
			if (machine == m)
				continue;
			machine->OnDisconnect(*m);
			m->OnDisconnect(*machine);
		}

		machines.erase(std::remove(machines.begin(), machines.end(), m), machines.end());
	}
}
