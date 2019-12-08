#include "luastateinspector.h"
#include <imgui.h>
#include "lua.hpp"
#include "lualib.h"
#include <string>
#include <sstream>

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

void LUINT::GUI::DrawLuaStateInspector(lua_State * state)
{
	if (!ImGui::Begin("State Inspector", nullptr, 0))
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
