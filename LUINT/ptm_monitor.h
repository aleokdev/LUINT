#pragma once
#include "machine.h"
#include <vector>

namespace LUINT::Machines
{
	
	// Persistent Text Matrix Monitor (PTM)
	struct PTMMonitor : public Machine
	{
		PTMMonitor(Data::SessionData& _session, std::string _name, Network* _network) : Machine(_session, _name, _network)
		{
			for (int i = 0; i < height; i++)
			{
				rows.emplace_back((char*)malloc(width+1));
				rows[i][width] = '\0';
			}

			Fill(' ', 1, 1, width, height);
		}

		~PTMMonitor()
		{
			for (auto& row : rows)
			{
				free(row);
			}
		}

		GENERATE_MACHINEINFO(PTMMonitor, (MachineInfo{ "PTM Monitor", "aleok studios",
			"Persistent Text Matrix Monitor.\n"
			"Can be given draw commands from a Processing Unit. Consists only of ASCII text characters.\nDoesn't need to be given instructions every tick; "
			"Since it's persistent, all data set will persist until the monitor is turned off or the data is changed."}));

	protected:
		void RenderWindow() override;
		void ImplementLua(lua_State* state, sol::table& proxy_table) override;

		ImGuiWindowFlags GetWindowFlags() override { return ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize; }

	private:
		void f_set(sol::this_state s, std::string str, int x, int y);
		void f_fill(sol::this_state s, std::string str, int x, int y, int w, int h);
		void Fill(char c, int x, int y, int w, int h);

		unsigned int width = 90;
		unsigned int height = 40;

		std::vector<char*> rows;
	};
}