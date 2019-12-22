#include "button.h"

namespace LUINT::Machines
{
	void Button::RenderWindow()
	{
		ImGui::Selectable("Button");
		if (ImGui::IsItemClicked(0))
		{
			network->SendEvent("button_pressed", uid);
			pressed = true;
		}
		if (ImGui::IsMouseReleased(0) && pressed)
		{
			network->SendEvent("button_released", uid);
			pressed = false;
		}
		ImGui::SetWindowSize(ImGui::GetContentRegionAvail(), ImGuiCond_Appearing);
	}
}
