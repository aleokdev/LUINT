#include "keyboard.h"

namespace LUINT::Machines
{
	void KeyboardRelay::RenderWindow()
	{
		ImGuiIO& io = ImGui::GetIO();
		for (int i = 0; i < 512; i++)
		{
			if (io.KeysDown[i])
			{
				if (io.KeysDownDuration[i] == 0.f)
				{
					// Key has just been pressed
					network->BroadcastEvent("key_pressed", uid,
						std::vector<sol::object>
						{
							sol::make_object(network->network_state, i)
						}
					);
				}
			}
			else
			{
				if (io.KeysDownDurationPrev[i] != -1.f)
				{
					// Key has just been released
					network->BroadcastEvent("key_released", uid,
						std::vector<sol::object>
					{
						sol::make_object(network->network_state, i)
					}
					);
				}
			}
		}
	}
}