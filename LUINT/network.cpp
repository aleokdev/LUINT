#include "network.h"

namespace LUINT
{
	void Network::SendEvent(std::string name, sol::table args)
	{
		OnEvent(name, args);
	}
}
