#pragma once
#include "api/machine.hpp"

namespace LUINT::Machines
{
	struct Button : public Machine
	{
		Button(LUINT::Data::SessionData& _session, std::string _name, Network* _network) : Machine(_session, _name, _network) {};

		GENERATE_MACHINEINFO(Button, (MachineInfo{ "Button", "aleok studios", "A button that sends button_pressed and button_released events.", Interfaces::get_Button() }));

	protected:
		void RenderWindow() override;

	private:
		bool pressed = false;
	};
}