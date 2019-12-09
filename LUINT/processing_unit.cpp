#include "machine.h"
#include "gui.h"
#include "lua.hpp"
#include "lualib.h"
#include <imgui.h>

namespace LUINT::Machines
{
	ProcessingUnit::ProcessingUnit(Data::SessionData& _session, std::string _name, std::string _manufacturer) : StateMachine(_session, _name, _manufacturer)
	{
		luaopen_string(state);
		luaopen_base(state);
		luaopen_table(state);
	}

	void ProcessingUnit::RenderMenuItems()
	{
		if (ImGui::BeginMenu("Debug"))
		{
			ImGui::MenuItem("State Inspector", NULL, &showStateInspector);
			ImGui::EndMenu();
		}
	}

	void ProcessingUnit::RenderChildWindows()
	{
		if (showStateInspector)
		{
			LUINT::GUI::DrawLuaStateInspector(*this, &showStateInspector);
		}
	}

	void ProcessingUnit::RenderWindow()
	{
		ImGui::AlignTextToFramePadding();
		ImGui::TextWrapped("Machine currently powered down.");
	}
}