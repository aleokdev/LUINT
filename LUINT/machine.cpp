#include "machine.h"
#include "lua.hpp"
#include <imgui.h>
#include "gui.h"
#include <algorithm>
#include "hostdata.h"
#include <iostream>

using namespace LUINT;

namespace LUINT::Machines
{
	Machine::Machine(Data::SessionData& _session, std::string _name) : session(&_session), name(_name), uid(UID::generate())
	{
	}

	Machine::~Machine()
	{
		// Remove the machine from any connections.

		int i = 0;
		for (auto& connection : session->connections)
		{
			if (connection.first == this || connection.second == this)
			{
				if (connection.first == this)
					connection.second->OnDisconnect(*this);
				else
					connection.first->OnDisconnect(*this);
				session->connections.erase(session->connections.begin() + i);
			}

			i++;
		}
	}

#pragma region Rendering

	void Machine::ShowMachineInfo()
	{
		{
			char machineInfoName[64];
			sprintf_s(machineInfoName, 64, "Machine Information###%s", uid.as_string().c_str());
			if (!ImGui::Begin(machineInfoName, &showMachineInfo))
			{
				ImGui::End();
				return;
			}
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

		ImGui::Text("Machine manufacturer: %s", get_info().manufacturer);

		ImGui::TextUnformatted("Description:");
		ImGui::TextWrapped(get_info().description);
		if (ImGui::CollapsingHeader("Advanced information"))
		{
			ImGui::Text("UID: %s", uid.as_string().c_str());
			ImGui::Text("Lua version: %s", LUA_VERSION);
		}
		ImGui::End();
	}
	
	void Machine::Render()
	{
		ImGuiIO& io = ImGui::GetIO();

		if (showMachineInfo)
			ShowMachineInfo();

		RenderChildWindows();

		ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);

		char buf[MAX_MACHINENAME_LENGTH + 32];
		sprintf_s(buf, MAX_MACHINENAME_LENGTH + 32, "%s###m%s", GetWindowName().c_str(), uid.as_string().c_str()); // Use ### to have an unique identifier (even when the machine changes its name)

		if (!ImGui::Begin(buf, nullptr, ImGuiWindowFlags_MenuBar))
		{
			// Early out if the window is collapsed, as an optimization.
			ImGui::End();
			return;
		}

		windowPos = ImGui::GetWindowPos();
		windowSize = ImGui::GetWindowSize();

		if (ImGui::BeginMenuBar())
		{
			RenderMenuItems();
			AddDefaultMenuItems();
			ImGui::EndMenuBar();
		}

		RenderWindow();

		if (session->connecting != nullptr)
		{
			if (ImGui::IsWindowHovered())
			{
				ImVec2 min = ImGui::GetWindowPos();
				ImVec2 max = ImVec2(min.x + ImGui::GetWindowSize().x, min.y + ImGui::GetWindowSize().y);
				ImGui::GetForegroundDrawList()->AddRect(min, max, ImGui::GetColorU32(ImGuiCol_PlotHistogramHovered), 3, 15, 5.0f);

				if (ImGui::IsMouseClicked(0))
				{
					if (session->connecting == this)
					{
						showCannotConnectToItselfTooltip = true;
					}
					else
					{
						// Connect to the other machine
						session->connections.emplace_back(std::pair<Machine*, Machine*>(session->connecting, this));
						session->connecting->OnConnect(*this);
						this->OnConnect(*session->connecting);
						session->connecting = nullptr;
					}
				}

				if (ImGui::IsMouseReleased(0))
					showCannotConnectToItselfTooltip = false;
			}
		}

		if (showCannotConnectToItselfTooltip)
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted("Cannot connect a machine to itself.");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		ImGui::End();
	}

	void Machine::RenderWindow()
	{
		ImGui::TextWrapped("This is a default render text -- Override Machine::RenderWindow to have control of what is shown here!");
	}

	void Machine::AddDefaultMenuItems()
	{
		if (ImGui::BeginMenu("Connections"))
		{
			if (session->connecting == this)
			{
				if (ImGui::MenuItem("Connect", "Ctrl+LMB", true))
				{
					session->connecting = nullptr;
				}
			}
			else if (session->connecting == nullptr)
			{
				if (ImGui::MenuItem("Connect", "Ctrl+LMB", false))
				{
					session->connecting = this;
				}
			}
			else
			{
				ImGui::MenuItem("Connect", "Ctrl+LMB", false, false);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("About"))
		{
			ImGui::MenuItem("Machine Information", "Ctrl+I", &showMachineInfo);
			ImGui::EndMenu();
		}
	}

#pragma endregion Rendering

	StateMachine::StateMachine(Data::SessionData& _session, std::string _name) : Machine(_session, _name)
	{
		state = luaL_newstate();
	}

	Monitor::Monitor(Data::SessionData& _session, std::string _name) : Machine(_session, _name)
	{
	}

	void thing()
	{
		std::cout << "works";
	}

	void LED::ImplementLua(lua_State * state, sol::table&& proxy_table)
	{
		sol::state_view lua(state);

		proxy_table.set_function("set_state", &LED::f_set_state, this); //Why doesn't this compile???
		proxy_table.set_function("get_state", &LED::f_get_state, this);
	}

	void LED::RenderWindow()
	{
		ImGui::Text(turnedOn ? "On" : "Off");
	}

	void Keyboard::RenderWindow()
	{
		//ImGui::TextUnformatted("Type here!");
		//ImGuiIO& io = ImGui::GetIO();

		//for (int i = 0; i < 512; i++)
		//{
		//	if (io.KeysDownDuration[i] == 0.f)
		//	{
		//		// Just pressed key!
		//		for (auto& connection : session->connections)
		//		{
		//			if (connection.first == this)
		//			{

		//			}
		//			if (connection.second == this)
		//			{
		//				
		//			}
		//		}
		//	}
		//}
	}
}
