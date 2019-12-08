#pragma once

struct lua_State;

namespace LUINT::GUI
{
	void DrawLuaStateInspector(lua_State* state);
}