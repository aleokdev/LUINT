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
			ImGui::MenuItem("Terminal", NULL, &showTerminal);
			ImGui::EndMenu();
		}
	}

	void ProcessingUnit::RenderChildWindows()
	{
		if (showStateInspector)
		{
			LUINT::GUI::DrawLuaStateInspector(*this, &showStateInspector);
		}

		if (showTerminal)
		{
			RenderTerminal();
		}
	}

	void ProcessingUnit::RenderWindow()
	{
		ImGui::AlignTextToFramePadding();
		ImGui::TextWrapped("Machine currently powered down.");
	}

	void ProcessingUnit::RenderTerminal()
	{
		char buf[64];
		sprintf_s(buf, 64, "%s's Terminal###%s", name.c_str(), uid.as_string().c_str());
		if (!ImGui::Begin(buf, &showTerminal))
		{
			ImGui::End();
			return;
		}

		ImGui::BeginChild(uid.as_string().c_str(), ImVec2(0, -200));
		ImGui::Text("This is a terminal!");
		ImGui::EndChild();

		ImGui::InputTextWithHint("", "Input command...", terminalBuffer, terminalBufferSize);

		ImGui::End();
	}
}