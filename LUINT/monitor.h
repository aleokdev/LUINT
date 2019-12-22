#pragma once
#include "machine.h"

namespace LUINT::Machines
{
	struct Monitor : public Machine
	{
		Monitor(Data::SessionData& _session, std::string _name, Network* _network) : Machine(_session, _name, _network) {}

		GENERATE_MACHINEINFO(Monitor, (MachineInfo{ "Monitor", "aleok studios", "Shows data from a processing unit." }));
	};
}