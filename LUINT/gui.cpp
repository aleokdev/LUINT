#include "gui.h"
#include <imgui.h>
#include "lua.hpp"
#include "lualib.h"
#include <string>
#include <sstream>
#include "machine.h"
#include "hostdata.h"
#include "luainterface.h"

void drawLuaStateInspectorTable(lua_State * state)
{
	ImGui::Indent();
	lua_pushnil(state);
	while (lua_next(state, -2) != 0) { // key(-1) is replaced by the next key(-1) in table(-2)
		/* uses 'key' (at index -2) and 'value' (at index -1) */

		const char* keyName;
		const char* valueData;

		// Duplicate the values before converting them to string because tolstring can affect the actual data in the stack
		{
			lua_pushvalue(state, -2);
			keyName = lua_tostring(state, -1);
			lua_pop(state, 1);
		}

		{
			lua_pushvalue(state, -1);
			valueData = lua_tostring(state, -1);

			std::string final_string;
			if (valueData == nullptr)
			{
				// lua_tolstring returns NULL if the value is not a number or a string, so let's get the address instead.
				const void* address = lua_topointer(state, -1);
				std::ostringstream addressStr;
				addressStr << address;

				final_string.append("<Data @ 0x");
				final_string.append(addressStr.str());
				final_string.append(">");
				valueData = final_string.c_str();
			}

			lua_pop(state, 1); // Remove duplicate

			if (lua_istable(state, -1))
			{
				if (ImGui::CollapsingHeader(keyName))
					drawLuaStateInspectorTable(state);
				else
				{
					/* removes 'value'; keeps 'key' for next iteration */
					lua_pop(state, 1); // remove value(-1), now key on top at(-1)
				}
			}
			else
			{
				ImGui::Text("%s - %s", keyName, valueData);

				/* removes 'value'; keeps 'key' for next iteration */
				lua_pop(state, 1); // remove value(-1), now key on top at(-1)
			}
		}
	}
	lua_pop(state, 1); // remove starting table val
	ImGui::Unindent();
}

void LUINT::GUI::DrawLuaStateInspector(lua_State * state, bool* p_open)
{
	if (!ImGui::Begin("State Inspector", p_open, 0))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	if (ImGui::CollapsingHeader("GLOBALS"))
	{
		lua_pushglobaltable(state);
		drawLuaStateInspectorTable(state);
	}

	ImGui::End();
}

void LUINT::GUI::DrawLuaStateInspector(LUINT::Machines::StateMachine& machine, bool* p_open)
{
	char buf[64];
	sprintf_s(buf, 64, "State Inspector of %s###s%s", machine.name.c_str(), machine.uid.as_string().c_str());
	if (!ImGui::Begin(buf, p_open, 0))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	if (ImGui::CollapsingHeader("GLOBALS"))
	{
		lua_pushglobaltable(machine.get_state());
		drawLuaStateInspectorTable(machine.get_state());
	}

	ImGui::End();
}

void LUINT::GUI::DrawMainMenuBar(LUINT::Data::SessionData& session)
{
	static bool showDemo = false;
	static bool showMachineMenu = false;
	ImGuiIO& io = ImGui::GetIO();

	if (!ImGui::BeginMainMenuBar())
	{
		ImGui::EndMainMenuBar();
		return;
	}
	if (ImGui::BeginMenu("Create"))
	{
		using namespace LUINT::Machines;
		ImGui::MenuItem("New machine...", "Ctrl+N", &showMachineMenu);

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Debug"))
	{
		ImGui::MenuItem("Show Demo Window", "Ctrl+D", &showDemo);

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

	if(showDemo)
		ImGui::ShowDemoWindow(&showDemo);

	if (showMachineMenu)
		DrawMachineMenu(session, &showMachineMenu);
}

void LUINT::GUI::DrawConnections(LUINT::Data::SessionData & session)
{
	for (auto& connection : session.connections)
	{
		ImVec2 first_min = connection.first->get_window_pos();
		ImVec2 first_max = ImVec2(first_min.x + connection.first->get_window_size().x, first_min.y + connection.first->get_window_size().y);
		ImVec2 second_min = connection.second->get_window_pos();
		ImVec2 second_max = ImVec2(second_min.x + connection.second->get_window_size().x, second_min.y + connection.second->get_window_size().y);

		ImVec2 start_point;
		ImVec2 end_point;
		if (second_min.x > first_max.x)
		{ // second window is to the right of the other one
			start_point.x = first_max.x;
			end_point.x = second_min.x;
		}
		else if (second_max.x < first_min.x)
		{ // second window is to the left of the other one
			start_point.x = first_min.x;
			end_point.x = second_max.x;
		}
		else
		{ // second and first window are connected by a vertical line
			// Do a nice vertical line in between the minimum of the second window and the maximum of the first one
			start_point.x = second_min.x + (first_max.x - second_min.x) / 2;
			end_point.x = start_point.x;
		}

		if (second_min.y > first_max.y)
		{ // second window is to the bottom of the other one
			start_point.y = first_max.y;
			end_point.y = second_min.y;
		}
		else if (second_max.y < first_min.y)
		{ // second window is to the top of the other one
			start_point.y = first_min.y;
			end_point.y = second_max.y;
		}
		else
		{ // second and first window are connected by a horizontal line
			// Do a nice horizontal line in between the minimum of the second window and the maximum of the first one
			start_point.y = second_min.y + (first_max.y - second_min.y) / 2;
			end_point.y = start_point.y;
		}

		ImGui::GetBackgroundDrawList()->AddLine(start_point, end_point, ImGui::GetColorU32(ImGuiCol_ButtonActive), 5.0);
	}
}

struct encapsulated_getallmachineinfo
{
	template<typename MachineType>
	static void callback()
	{
		int this_index = LUINT::Machines::allMachineTypes.get_type_index<MachineType>();
		if (ImGui::Selectable(MachineType::static_info.name, selectedIndex == this_index))
		{
			selectedIndex = this_index;
			info_selected = &MachineType::static_info;
		}
	}

	inline static int selectedIndex = 0;
	inline static const LUINT::Machines::MachineInfo* info_selected = nullptr;
};

struct encapsulated_createmachine
{
	template<typename MachineType>
	static void callback()
	{
		if (&MachineType::static_info == machine_to_create)
			session->machines.emplace_back(std::make_unique<MachineType>(*session, MachineType::static_info.name));
	}

	inline static LUINT::Data::SessionData* session;
	inline static const LUINT::Machines::MachineInfo* machine_to_create = nullptr;
};

void LUINT::GUI::DrawMachineMenu(LUINT::Data::SessionData& session, bool* p_open)
{
	ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Add machine...", p_open, 0))
	{
		// Left panel
		ImGui::BeginChild("Left panel", ImVec2(150, 0), true);
		LUINT::Machines::allMachineTypes.eval_for_each<encapsulated_getallmachineinfo>();
		ImGui::EndChild();
		ImGui::SameLine();

		// right
		if (encapsulated_getallmachineinfo::info_selected == nullptr)
		{
			ImGui::End();
			return;
		}
		ImGui::BeginGroup();
		ImGui::BeginChild("Machine Preview", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
		ImGui::Text("%s", encapsulated_getallmachineinfo::info_selected->name);
		ImGui::Separator();
		if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Description"))
			{
				ImGui::TextWrapped(encapsulated_getallmachineinfo::info_selected->description);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Interface"))
			{
				ImGui::TextUnformatted("Lua Interface: ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 0.9f, 0.6f, 1), encapsulated_getallmachineinfo::info_selected->interface.name);
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1));
				ImGui::TextWrapped(encapsulated_getallmachineinfo::info_selected->interface.description);
				ImGui::PopStyleColor();
				ImGui::Separator();
				for (const LUINT::Machines::LuaInterfaceFunction& function : encapsulated_getallmachineinfo::info_selected->interface.functions)
				{
					ImGui::Text("function %s(", function.name);
					for (int i = 0; i < function.arguments.size(); i++)
					{
						ImGui::SameLine();
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.9f, 0.6f, 1));
						ImGui::TextUnformatted(function.arguments[i].name);
						ImGui::PopStyleColor();
						if (i != function.arguments.size() - 1)
						{
							ImGui::SameLine();
							ImGui::TextUnformatted(",");
						}
					}
					ImGui::SameLine();
					ImGui::TextUnformatted(")");

					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.9f, 0.9f, 1));
					ImGui::TextUnformatted("->");
					ImGui::SameLine();
					ImGui::PopStyleColor();
					ImGui::Indent();
					ImGui::TextWrapped(function.returns);
					ImGui::Unindent();

					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1));
					ImGui::Indent();
					ImGui::TextWrapped(function.description);
					ImGui::Unindent();
					ImGui::PopStyleColor();
				}

				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::EndChild();
		if (ImGui::Button("Create"))
		{
			if (encapsulated_getallmachineinfo::info_selected != nullptr)
			{
				encapsulated_createmachine::session = &session;
				encapsulated_createmachine::machine_to_create = encapsulated_getallmachineinfo::info_selected;
				LUINT::Machines::allMachineTypes.eval_for_each<encapsulated_createmachine>();
				*p_open = false;
			}
		}
		ImGui::EndGroup();
	}
	ImGui::End();
}