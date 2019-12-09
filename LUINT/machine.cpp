#include "machine.h"
#include "lua.hpp"
#include <imgui.h>
#include "gui.h"
#include <algorithm>
#include "hostdata.h"

using namespace LUINT;

namespace LUINT::Machines
{
	Machine::Machine(Data::SessionData& _session, std::string _name, std::string _manufacturer) : session(&_session), name(_name), manufacturer(_manufacturer), uid(UID::generate())
	{
	}

	Machine::~Machine()
	{
		// Remove the machine from any connections.

		int i = 0;
		for (auto& connection : session->connections)
		{
			if (connection.first == this || connection.second == this)
				session->connections.erase(session->connections.begin() + i);

			i++;
		}
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

#pragma region Rendering

	bool Machine::BeginWindow(ImGuiWindowFlags flags)
	{
		char buf[MAX_MACHINENAME_LENGTH + 32];
		sprintf_s(buf, MAX_MACHINENAME_LENGTH + 32, "%s###m%s", name.c_str(), uid.as_string().c_str()); // Use ### to have an unique identifier (even when the machine changes its name)

		return ImGui::Begin(buf, nullptr, flags);
	}

	void Machine::Render()
	{
		ImGuiIO& io = ImGui::GetIO();

		ImGui::SetNextWindowSize(ImVec2(400, 400));
		if (!this->BeginWindow(ImGuiWindowFlags_MenuBar))
		{
			// Early out if the window is collapsed, as an optimization.
			ImGui::End();
			return;
		}

		if (ImGui::BeginMenuBar())
		{
			RenderMenuItems();
			AddAboutMenuItem();
			ImGui::EndMenuBar();
		}

		RenderWindow();

		ImGui::End();
	}

	void Machine::RenderWindow()
	{
		ImGui::TextWrapped("This is a default render text -- Override Machine::RenderWindow to have control of what is shown here!");
	}

	void Machine::AddAboutMenuItem()
	{
		if (ImGui::BeginMenu("About"))
		{
			ImGui::MenuItem("Machine Information", "Ctrl+I", &showMachineInfo);
			ImGui::EndMenu();
		}

		if (showMachineInfo)
		{
			{
				char machineInfoName[64];
				sprintf_s(machineInfoName, 64, "Machine Information###%s", uid.as_string().c_str());
				ImGui::Begin(machineInfoName, &showMachineInfo);
			}

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
			ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1), get_description().c_str());
			if (ImGui::CollapsingHeader("Advanced information"))
			{
				ImGui::Text("UID: %s", uid.as_string().c_str());
				ImGui::Text("Lua version: %s", LUA_VERSION);
			}
			ImGui::End();
		}
	}

	void ProcessingUnit::RenderMenuItems()
	{
		if (ImGui::BeginMenu("Debug"))
		{
			ImGui::MenuItem("State Inspector", NULL, &showStateInspector);
			ImGui::EndMenu();
		}
	}

	void ProcessingUnit::RenderWindow()
	{
		ImGui::AlignTextToFramePadding();
		ImGui::TextWrapped("Machine currently powered down.");

		if (showStateInspector)
		{
			LUINT::GUI::DrawLuaStateInspector(state);
		}
	}

#pragma endregion Rendering

	StateMachine::StateMachine(Data::SessionData& _session, std::string _name, std::string _manufacturer) : Machine(_session, _name, _manufacturer)
	{
		state = luaL_newstate();
	}

	ProcessingUnit::ProcessingUnit(Data::SessionData& _session, std::string _name, std::string _manufacturer) : StateMachine(_session, _name, _manufacturer)
	{
		luaopen_string(state);
		luaopen_base(state);
		luaopen_table(state);
	}

	Monitor::Monitor(Data::SessionData& _session, std::string _name, std::string _manufacturer) : Machine(_session, _name, _manufacturer)
	{
	}

	Terminal::Terminal(Data::SessionData& _session, std::string _name, std::string _manufacturer) : Machine(_session, _name, _manufacturer)
	{
	}
}
