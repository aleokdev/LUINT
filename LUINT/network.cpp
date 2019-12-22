#include "network.h"
#include <algorithm>
#include "machine.h"

namespace LUINT
{
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
