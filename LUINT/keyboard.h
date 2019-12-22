#pragma once
#include "machine.h"

namespace LUINT::Machines
{
	struct Keyboard : public Machine
	{
		Keyboard(LUINT::Data::SessionData& _session, std::string _name, Network* _network) : Machine(_session, _name, _network) {};

		GENERATE_MACHINEINFO(Keyboard, (MachineInfo{ "Keyboard", "aleok studios", "Simple machine that sends key_pressed and key_released events to state machines connected.", Interfaces::get_Keyboard() }));

	protected:
		//void RenderWindow() override;
	};
}