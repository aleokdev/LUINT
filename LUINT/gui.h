#pragma once

struct lua_State;

namespace LUINT::Machines
{
	struct StateMachine;
}

namespace LUINT::Data
{
	struct SessionData;
}

namespace LUINT::GUI
{
	void DrawLuaStateInspector(lua_State* state, bool* p_open);
	void DrawLuaStateInspector(LUINT::Machines::StateMachine& machine, bool* p_open);
	void DrawMainMenuBar(LUINT::Data::SessionData& session);
	void DrawConnections(LUINT::Data::SessionData& session);
}