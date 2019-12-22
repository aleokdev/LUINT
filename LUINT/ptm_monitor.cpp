#include "ptm_monitor.h"

namespace LUINT
{
	void Machines::PTMMonitor::RenderWindow()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		for (int i = 0; i < height; i++)
		{
			ImGui::TextUnformatted(rows[i]);
		}
		ImGui::PopStyleVar();

		ImGui::SetWindowSize(ImGui::GetContentRegionAvail(), ImGuiCond_Appearing);
	}

	void Machines::PTMMonitor::ImplementLua(lua_State * state, sol::table && proxy_table)
	{
		sol::state_view lua(state);

		proxy_table.set_function("set", &PTMMonitor::f_set, this);
		proxy_table.set_function("fill", &PTMMonitor::f_fill, this);
	}

	void Machines::PTMMonitor::f_set(std::string str, int x, int y)
	{
		if (x <= 0 || x > width || y <= 0 || y > height || str.length() == 0)
			return;

		rows[y - 1][x - 1] = str[0];
	}

	void Machines::PTMMonitor::f_fill(std::string str, int x, int y, int w, int h)
	{
		if (x <= 0 || x > width || y <= 0 || y > height || str.length() == 0 ||
			w <= 0 || h <= 0 || x + w > width+1 || y + h > height+1)
			return;

		for(int ix = x; ix < x + w; ix++)
			for(int iy = y; iy < y + h; iy++)
				rows[iy - 1][ix - 1] = str[0];
	}
}
