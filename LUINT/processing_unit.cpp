#include "machine.h"
#include "gui.h"
#include "lua.hpp"
#include "lualib.h"
#include <imgui.h>
#include <iostream>
#include <string>

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
		sprintf_s(buf, 64, "%s's Terminal###t%s", name.c_str(), uid.as_string().c_str());
		ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin(buf, &showTerminal))
		{
			ImGui::End();
			return;
		}

		ImGui::BeginChild(uid.as_string().c_str(), ImVec2(0, 200));
		for (std::string& str : terminalLog)
		{
			const char* c_str = str.c_str();
			bool pop_color = false;
			if (strncmp(c_str, ">>> ", 4) == 0)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1));
				pop_color = true;
			}
			else if (strncmp(c_str, "[error]", 4) == 0)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.2f, 0.2f, 1));
				pop_color = true;
			}
			else if (strncmp(c_str, "[info]", 4) == 0)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 0.9f, 0.8f, 1));
				pop_color = true;
			}
			ImGui::TextUnformatted(str.c_str());

			if (pop_color)
				ImGui::PopStyleColor();
		}
		ImGui::EndChild();

		ImGui::Separator();
		ImGui::SetNextItemWidth(ImGui::GetWindowWidth());
		if (ImGui::InputTextWithHint("", "Input command...", terminalBuffer, terminalBufferSize, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			{
				char buf[128];
				sprintf_s(buf, 128, ">>> %s", terminalBuffer);
				terminalLog.emplace_back(std::string(buf));
			}
			std::cout << terminalBuffer;
			if (luaL_loadstring(state, terminalBuffer))
			{
				terminalLog.emplace_back(std::string("[error] Syntax Error!"));
			}
			else
			{
				terminalLog.emplace_back(std::string("[info] Syntax OK!"));
			}

			lua_pop(state, 1);

			terminalBuffer[0] = '\0';
		}

		ImGui::End();
	}
}