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

	void Machines::PTMMonitor::OnChangeNetwork(Network* before, Network* next)
	{
		if (before)
		{
			before->OnEvent.remove_observer(uid);
		}

		next->OnEvent.add_observer(uid, [this](Network::Event e) { ProcessEvent(e); });
	}

	void Machines::PTMMonitor::ProcessEvent(Network::Event e)
	{
		if (e.name == "set")
		{
			if (e.args.size() != 3)
				return;

			if (!e.args[0].is<std::string>() || !e.args[1].is<int>() || !e.args[2].is<int>())
				return;

			if (e.args[0].as<std::string>().size() == 0)
				return;

			Set(e.args[0].as<std::string>()[0], e.args[1].as<int>(), e.args[2].as<int>());
		}
		else if (e.name == "fill")
		{
			if (e.args.size() != 5)
				return;

			if (!e.args[0].is<std::string>() || !e.args[1].is<int>() || !e.args[2].is<int>() || !e.args[3].is<int>() || !e.args[4].is<int>())
				return;

			if (e.args[0].as<std::string>().size() == 0)
				return;

			Fill(e.args[0].as<std::string>()[0], e.args[1].as<int>(), e.args[2].as<int>(), e.args[3].as<int>(), e.args[4].as<int>());
		}
	}

	void Machines::PTMMonitor::Set(char c, int x, int y)
	{
		if (x <= 0 || x > width || y <= 0 || y > height)
			return;

		rows[y - 1][x - 1] = c;
	}

	void Machines::PTMMonitor::Fill(char c, int x, int y, int w, int h)
	{
		if (x <= 0 || x > width || y <= 0 || y > height ||
			w <= 0 || h <= 0 || x + w > width + 1 || y + h > height + 1)
			return;

		for (int ix = x; ix < x + w; ix++)
			for (int iy = y; iy < y + h; iy++)
				rows[iy - 1][ix - 1] = c;
	}
}
