#include "LED.h"

namespace LUINT::Machines
{
	void LED::ImplementLua(lua_State * state, sol::table& proxy_table)
	{
		sol::state_view lua(state);

		proxy_table.set_function("set_state", &LED::f_set_state, this);
		proxy_table.set_function("get_state", &LED::f_get_state, this);
	}

	void LED::RenderWindow()
	{
		ImGui::Text(turnedOn ? "On" : "Off");
	}
}