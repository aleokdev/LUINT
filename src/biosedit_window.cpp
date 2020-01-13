#include "biosedit_window.h"
#include <imgui.h>

namespace LUINT::GUI
{
	void BIOSEditWindow::Draw(bool * p_open)
	{
		char buf[32];
		sprintf_s(buf, 32, "BIOS Editor###%s", uid.as_string("%x").c_str());
		ImGui::SetNextWindowSize({ 500, 500 }, ImGuiCond_FirstUseEver);
		if (!ImGui::Begin(buf, p_open, ImGuiWindowFlags_NoScrollbar))
		{
			ImGui::End();
			return;
		}

		ImVec2 size = ImGui::GetContentRegionAvail();
		size.y -= 30;
		ImGui::InputTextMultiline("##bios_text", text_buffer, MAX_BIOS_SIZE, size);
		if (ImGui::Button("OK"))
			bios->assign(text_buffer);

		ImGui::End();
	}
}
