#include "LED.h"

namespace LUINT::Machines
{
	void LED::OnChangeNetwork(Network* before, Network* next)
	{
		if (before)
		{
			before->OnEvent.remove_observer(uid);
		}

		next->OnEvent.add_observer(uid, [this](Network::Event e) { ProcessEvent(e); });
	}

	void LED::ProcessEvent(Network::Event e)
	{
		if (e.name == "get_state")
		{
			network->SendEvent(e.sender_uid, e.name, uid, std::vector<sol::object> { sol::make_object(e.state, turnedOn) });
		}
		else if (e.name == "set_state")
		{
			if (e.args.size() != 1)
				return;

			if (!e.args[0].is<bool>())
				return;

			turnedOn = e.args[0].as<bool>();
		}
	}

	void LED::RenderWindow()
	{
		ImGui::Text(turnedOn ? "On" : "Off");
	}
}