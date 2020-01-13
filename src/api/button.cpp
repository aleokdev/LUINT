#include "machines/button.hpp"

namespace LUINT::Machines
{
	void Button::RenderWindow()
	{
		ImGui::Selectable("Button");
		if (ImGui::IsItemClicked(0))
		{
			network->BroadcastEvent("button_pressed", uid, std::vector<sol::object>{});
			pressed = true;
		}
		if (ImGui::IsMouseReleased(0) && pressed)
		{
			network->BroadcastEvent("button_released", uid, std::vector<sol::object>{});
			pressed = false;
		}
		ImGui::SetWindowSize(ImGui::GetContentRegionAvail(), ImGuiCond_Appearing);
	}
}
