#pragma once
#include "observable.h"
#include "sol.hpp"
#include "UID.h"

namespace LUINT
{
	namespace Machines
	{
		struct Machine;
	}

	struct Network
	{
		void SendEvent(std::string name, sol::table args);
		lel::observable<std::string, sol::table> OnEvent;
		const std::vector<Machines::Machine*>& get_machines() { return machines; }
		void add_machine(Machines::Machine* m);
		void remove_machine(Machines::Machine* m);
		UID uid = UID::generate();

	private:
		std::vector<Machines::Machine*> machines;
	};
}