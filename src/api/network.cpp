#include "api/network.hpp"
#include <algorithm>
#include "api/machine.hpp"

namespace LUINT
{
	bool Network::is_connected(Machines::Machine * m) const
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
