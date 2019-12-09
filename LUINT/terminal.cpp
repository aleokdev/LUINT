#include "machine.h"
#include <imgui.h>


namespace LUINT::Machines
{
	std::string Terminal::GetWindowName()
	{
		if (machineConnectedTo)
		{
			char windowName[64];
			sprintf_s(windowName, 64, "%s's Terminal", machineConnectedTo->name.c_str());
			return std::string(windowName);
		}
		else
			return std::string("Disconnected Terminal");
	}

	void Terminal::RenderWindow()
	{
		ImGui::BeginChild(uid.as_string().c_str(), ImVec2(0, -200));

		ImGui::Text("This is a terminal!");

		ImGui::EndChild();

		ImGui::InputTextWithHint("", "Input command...", inputBuffer, inputBufferSize);
	}

	void Terminal::OnConnect(Machine& other)
	{

	}

	void Terminal::OnDisconnect(Machine& other)
	{

	}
}