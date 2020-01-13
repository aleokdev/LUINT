#pragma once
#include "api/observable.hpp"
#include "sol.hpp"
#include "api/UID.hpp"
#include "lua.h"
#include <unordered_map>

namespace LUINT
{
	namespace Machines
	{
		struct Machine;
	}

	struct Network
	{
		~Network() {}

		void BroadcastEvent(std::string name, UID sender, std::vector<sol::object> vals, lua_State* state = nullptr)
		{
			OnEvent(Event{ name, sender, vals, state });
		}

		void SendEvent(UID receiver, std::string name, UID sender, std::vector<sol::object> vals, lua_State* state = nullptr)
		{
			if (OnEvent.observers.find(receiver) == OnEvent.observers.end())
				throw std::runtime_error("Receiver doesn't exist in observer list");

			OnEvent.observers[receiver](Event{ name, sender, vals, state });
		}

		lua_State* network_state;

		struct Event
		{
			std::string name;
			UID sender_uid;
			std::vector<sol::object> args;
			lua_State* state;
		};

		lel::observable<Event> OnEvent;

		const std::vector<Machines::Machine*>& get_machines() { return machines; }
		void add_machine(Machines::Machine*);
		void remove_machine(Machines::Machine*);
		bool is_connected(Machines::Machine*) const;

		const UID& get_uid() const { return uid; }

	private:
		std::vector<Machines::Machine*> machines;
		std::unordered_map<UID, void*> definitions;
		UID uid = UID::generate();
	};
}