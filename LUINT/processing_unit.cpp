#include "processing_unit.h"
#include "gui.h"
#include "lua.hpp"
#include "lualib.h"
#include <imgui.h>
#include <iostream>
#include <string>
#include <sstream>
#include "sol.hpp"

namespace LUINT::Machines
{
	ProcessingUnit::ProcessingUnit(Data::SessionData& _session, std::string _name, Network* _network) : Machine(_session, _name, _network)
	{
		Setup();
	}

	void ProcessingUnit::Setup()
	{
		if (state)
		{
			network->try_set_default_lua_state(nullptr);
			network->remove_state(state);
			lua_close(state);
		}
		state = luaL_newstate();
		network->try_set_default_lua_state(state);
		network->add_state(state);

		sol::state_view lua(state);

		lua.open_libraries(sol::lib::coroutine, sol::lib::string, sol::lib::base, sol::lib::table, sol::lib::coroutine);

		// Computer functions
		sol::table t = lua.create_named_table("computer");
		t.set_function("ticks", &ProcessingUnit::f_ticks, this);
		t.set_function("shutdown", [this]() { after_tick = AfterTickAction::shutdown; });
		t.set_function("reboot", [this]() { after_tick = AfterTickAction::reboot; });
		t.set_function("push", [this](std::string event_name, sol::table params) { PushEvent(event_name, uid, params); });
		t.set_function("get_connections", &ProcessingUnit::GetNetworkUIDs, this);
		t.set_function("proxy", &ProcessingUnit::GetProxy, this);
	}

	sol::table ProcessingUnit::GetNetworkUIDs()
	{
		sol::table retval = sol::state_view(state).create_table();
		int i = 1;
		for (auto& machine : network->get_machines())
		{
			if (machine == this)
				continue;

			retval[i] = machine->uid.as_string("%08x");
			i++;
		}

		return retval;
	}

	sol::table ProcessingUnit::GetProxy(std::string uid)
	{
		if (proxies.find(uid) == proxies.end())
			return sol::nil;

		return proxies[uid];
	}

	void ProcessingUnit::ReconnectAll()
	{
		for (auto& machine : network->get_machines())
		{
			OnConnect(*machine);
		}
	}

	void ProcessingUnit::Startup()
	{
		sol::state_view lua(state);
		// Create main coroutine, set it to bios main function
		// todo: allow modifying bios
		
		// TODO: Replace #include with #embed_str, when MSVC supports it
		auto result = lua.do_string(
#include "monitor_test_bios.lua"
);
		
		if (!result.valid())
		{
			sol::error err = result;
			std::cout << "Fatal: BIOS couldn't load. Error:\n" << err.what() << std::endl;
			throw;
		}

		main_coroutine = std::make_unique<sol::coroutine>(lua["main"]);
		is_on = true;
		ticks_since_startup = 0;
		PushEvent("startup", uid, sol::lua_value(lua, sol::lua_nil));
	}

	void ProcessingUnit::Shutdown()
	{
		is_on = false;
		main_coroutine.reset();
		Setup();
		ReconnectAll();
	}

	void ProcessingUnit::Reboot()
	{
		is_on = false;
		Setup();
		ReconnectAll();
		Startup();
	}

	void ProcessingUnit::OnConnect(Machine & other)
	{
		sol::state_view lua(state);
		std::cout << "adding\n";
		std::string otherUID = other.uid.as_string("%08x");
		sol::table proxy = lua.create_table_with(
			"uid", otherUID,
			"name", other.name,
			"type", lua.create_table_with(
				"name", other.get_info().name,
				"description", other.get_info().description,
				"manufacturer", other.get_info().manufacturer,
				"interface", lua.create_table_with(
					"name", other.get_info().interface.name,
					"description", other.get_info().interface.description
				)
			)
		);
		other.ImplementLua(state, proxy);
		proxies.emplace(otherUID, proxy);

		if(is_on)
			PushEvent("on_connect", uid, lua.create_table_with(1, otherUID));
	}

	void ProcessingUnit::OnDisconnect(Machine & other)
	{
	}

	void ProcessingUnit::PushEvent(std::string name, UID sender, sol::lua_value parameters)
	{
		if (!is_on)
			return;

		sol::state_view lua(state);

		//std::cout << "pushing event named " << name << "\n";
		// First, push the event to latest_event
		eventQueue.push({ name, sender, parameters });
	}

	void ProcessingUnit::Tick()
	{
		sol::state_view lua(state);
		while(eventQueue.size() > 0)
		{
			auto event = eventQueue.front();
			eventQueue.pop();
			lua[sol::create_if_nil]["latest_event"] = lua.create_table_with(1, std::get<0>(event), 2, std::get<1>(event).as_string("%08x").c_str(), 3, std::get<2>(event));
			auto result = (*main_coroutine)(); // Continue executing the main coroutine

			if (!main_coroutine->runnable())
			{
				std::cout << "Main coroutine has finished executing." << std::endl;
				is_on = false;
			}
			if (!result.valid())
			{
				sol::error err = result;
				std::cout << "FATAL: Could not continue main coroutine:\n" << err.what() << std::endl;
			}
		}
	}

	void ProcessingUnit::RenderMenuItems()
	{
		sol::state_view lua(state);

		if (ImGui::MenuItem("Start up"))
			Startup();

		if (ImGui::MenuItem("Send signal") && is_on)
			PushEvent("test", uid, lua.create_table_with(1, "this is a test parameter"));

		if (ImGui::BeginMenu("Debug"))
		{
			ImGui::MenuItem("State Inspector", NULL, &showStateInspector);
			ImGui::MenuItem("Terminal", NULL, &showTerminal);
			ImGui::EndMenu();
		}
	}

	void ProcessingUnit::OnChangeNetwork(Network* prev, Network* next)
	{
		if (prev)
		{
			prev->OnEvent.remove_observer(uid);
			next->remove_state(state);
		}
		next->OnEvent.add_observer(uid, [this](std::string s, UID u, sol::lua_value v) { PushEvent(s, u, v); });
		next->add_state(state);
		next->try_set_default_lua_state(state);
	}

	void ProcessingUnit::RenderChildWindows()
	{
		if (showStateInspector)
		{
			LUINT::GUI::DrawLuaStateInspector(name.c_str(), state, &showStateInspector);
		}

		if (showTerminal)
		{
			RenderTerminal();
		}
	}

	void ProcessingUnit::RenderWindow()
	{
		ImGui::AlignTextToFramePadding();
		ImGui::TextWrapped(is_on? "Machine currently turned on." : "Machine currently powered down.");
		
		if (is_on)
		{
			time_since_last_tick += ImGui::GetIO().DeltaTime;
			if (time_since_last_tick > 1.f / ticks_per_second)
			{
				ticks_since_startup++;
				PushEvent("tick", uid, sol::lua_value(state, sol::lua_nil));
				Tick();
				time_since_last_tick = 0;
			}
		}

		switch (after_tick)
		{
			case(AfterTickAction::reboot):
				Reboot();
				after_tick = AfterTickAction::none;
				break;

			case(AfterTickAction::shutdown):
				Shutdown();
				after_tick = AfterTickAction::none;
				break;
		}
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

		ImGui::BeginChild(uid.as_string().c_str(), ImVec2(0, ImGui::GetWindowContentRegionMin().y - 60));
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
		//ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
		if (ImGui::InputTextWithHint("", "Input command...", terminalBuffer, terminalBufferSize, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			{
				char buf[128];
				sprintf_s(buf, 128, ">>> %s", terminalBuffer);
				terminalLog.emplace_back(std::string(buf));
			}
			lua_pushnil(state); // Push nil before loading function to know where to stop removing results from the stack.
			if (luaL_loadstring(state, terminalBuffer))
			{
				terminalLog.emplace_back(std::string("[error] Syntax Error!"));
			}
			else
			{
				if (lua_pcall(state, 0, LUA_MULTRET, 0) == 0)
				{
					// Process response until nil pushed earlier if successful.
					while (!lua_isnil(state, -1))
					{
						const char* result = lua_tostring(state, -1);

						if (result == nullptr)
						{
							if (lua_isboolean(state, -1))
							{
								terminalLog.emplace_back(std::string(lua_toboolean(state, -1) ? "true" : "false"));
							}
							else
							{
								// Result is not number, bool or string, so get address instead
								// TODO: Merge with NUINT::GUI::drawLuaStateInspectorTable's implementation
								const void* address = lua_topointer(state, -1);
								std::ostringstream addressStr;
								std::string final_string;
								addressStr << address;

								final_string.append("<Data @ 0x");
								final_string.append(addressStr.str());
								final_string.append(">");
								terminalLog.emplace_back(std::move(final_string));
							}
						}
						else if (strlen(result) == 0) // result is empty
						{
							terminalLog.emplace_back(std::string("<nil>"));
						}
						else
							terminalLog.emplace_back(std::string(result));
						lua_pop(state, 1);
					}
				}
				else
				{
					// Format exception as error if not successful.
					const char* exc = lua_tostring(state, -1);
					char full_exception[256];
					sprintf_s(full_exception, 256, "[error] %s", exc);
					terminalLog.emplace_back(std::string(full_exception));
					lua_pop(state, 1); // Pop exception
				}
				
			}

			lua_pop(state, 1); // Pop nil

			terminalBuffer[0] = '\0';
		}
		ImGui::SameLine();
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted("This is a processing unit terminal. You can directly set or get data from the machine from here, you just need to "
								   "input a valid Lua command. For getting values, do \"return <data>\" as if the input text were a function.\n\n"
								   "This should not be the main output device for your machine, since there is no function to log here. This is only "
								   "meant for debugging purposes. If you need an output device for your machine, use a Monitor instead.");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear"))
			terminalLog.clear();

		ImGui::End();
	}
}