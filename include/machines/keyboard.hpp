#pragma once
#include "api/machine.hpp"

namespace LUINT::Machines
{
	struct KeyboardRelay : public Machine
	{
		KeyboardRelay(LUINT::Data::SessionData& _session, std::string _name, Network* _network) : Machine(_session, _name, _network) {};

		GENERATE_MACHINEINFO(KeyboardRelay, (MachineInfo{ "Keyboard Relay", "aleok studios", "Simple machine that sends key_pressed and key_released events to state machines connected.", Interfaces::get_Keyboard() }));

	protected:
		void RenderWindow() override;
	};
}