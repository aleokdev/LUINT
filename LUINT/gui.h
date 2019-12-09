#pragma once

struct lua_State;

namespace LUINT::Data
{
	struct SessionData;
}

namespace LUINT::GUI
{
	void DrawLuaStateInspector(lua_State* state);
	void DrawMainMenuBar(LUINT::Data::SessionData& session);
}