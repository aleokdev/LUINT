#include "machine.h"
#include "lua.hpp"
#include <imgui.h>
#include "luastateinspector.h"

using namespace LUINT;

namespace LUINT::Machines
{
	Machine::Machine(std::string _name, std::string _manufacturer) : name(_name), manufacturer(_manufacturer), uid(UID::generate())
	{
	}

	void Machine::MountToCurrentTable(lua_State * state)
	{
		lua_pushstring(state, uid.as_string().c_str());
		lua_newtable(state);

		{
			lua_pushstring(state, "name");
			lua_pushstring(state, name.c_str());
			lua_settable(state, -3);

			lua_pushstring(state, "manufacturer");
			lua_pushstring(state, manufacturer.c_str());
			lua_settable(state, -3);

			lua_pushstring(state, "description");
			lua_pushstring(state, get_description().c_str());
			lua_settable(state, -3);

			lua_pushstring(state, "loaded");
			PushFunctionsToStack(state);
			lua_settable(state, -3);
		}

		lua_settable(state, -3);
	}

	void Machine::UnmountFromCurrentTable(lua_State * state)
	{
		// TODO: UnmountFromCurrentTable
	}

	void Machine::Render()
	{
		ImGuiIO& io = ImGui::GetIO();

		if (!ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_MenuBar))
		{
			// Early out if the window is collapsed, as an optimization.
			ImGui::End();
			return;
		}

		if (ImGui::BeginMenuBar())
		{
			AddAboutMenuItem();
			ImGui::EndMenuBar();
		}

		ImGui::PushItemWidth(ImGui::GetFontSize() * -12); // Negative number to get fixed width

		ImGui::TextWrapped("This is a default render text -- Override it to have control of what is shown here!");

		ImGui::End();
	}

	void Machine::AddAboutMenuItem()
	{
		static bool showMachineInfo = false;

		if (ImGui::BeginMenu("About"))
		{
			ImGui::MenuItem("Machine Information", "Ctrl + I", &showMachineInfo);
			ImGui::EndMenu();
		}

		if (showMachineInfo)
		{
			ImGui::Begin("Machine Information", &showMachineInfo);

			static bool editingName = false;
			if (editingName)
			{
				static char buf[32] = "";
				if (ImGui::InputTextWithHint("", "Input new machine name...", buf, 32, ImGuiInputTextFlags_EnterReturnsTrue))
				{
					name = buf;
					editingName = false;
				}
			}
			else
			{
				ImGui::Text("Machine name: %s", name.c_str());
				ImGui::SameLine();
				editingName = ImGui::Button("Change");
			}

			ImGui::Text("Machine manufacturer: %s", manufacturer.c_str());

			ImGui::TextUnformatted("Description:");
			ImGui::TextColored(ImVec4(92, 92, 92, 255), get_description().c_str());
			ImGui::End();
		}
	}

	StateMachine::StateMachine(std::string _name, std::string _manufacturer) : Machine(_name, _manufacturer)
	{
		state = luaL_newstate();
	}

	Computer::Computer(std::string _name, std::string _manufacturer) : StateMachine(_name, _manufacturer)
	{
		luaopen_string(state);
		luaopen_base(state);
		luaopen_table(state);
	}

	void Computer::Render()
	{
		static bool showStateInspector = false;

		ImGuiIO& io = ImGui::GetIO();

		ImGui::SetNextWindowSize(ImVec2(800, 600));
		char buf[MAX_MACHINENAME_LENGTH + 32];
		sprintf_s(buf, MAX_MACHINENAME_LENGTH + 32, "%s###m%s", name.c_str(), uid.as_string()); // Use ### to have an unique identifier (even when the machine changes its name)

		if (!ImGui::Begin(buf, nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar))
		{
			// Early out if the window is collapsed, as an optimization.
			ImGui::End();
			return;
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Debug"))
			{
				ImGui::MenuItem("State Inspector", NULL, &showStateInspector);
				ImGui::EndMenu();
			}
			AddAboutMenuItem();
			ImGui::EndMenuBar();
		}

		ImGui::AlignTextToFramePadding();
		ImGui::PushItemWidth(ImGui::GetFontSize() * -12); // Negative number to get fixed width
		ImGui::Text("Machine currently powered down.");

		ImGui::End();

		if (showStateInspector)
		{
			LUINT::GUI::DrawLuaStateInspector(state);
		}
	}
}
