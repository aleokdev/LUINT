#pragma once
#include "api/machine.hpp"
#include "api/network.hpp"

namespace LUINT::Machines
{
	struct LED : public Machine
	{
		LED(LUINT::Data::SessionData& _session, std::string _name, Network* _network) : Machine(_session, _name, _network)
		{
			OnChangeNetwork(nullptr, network);
		};

		GENERATE_MACHINEINFO(LED, (MachineInfo{ "LED", "aleok studios", "Simple machine that can turn on or off.", Interfaces::get_SimpleOutputDevice() }));

		struct lua_impl
		{
			void do_thing()
			{
				std::cout << "thing";
			}
		};

		inline void set_state(sol::this_state s, bool new_state)
		{
			turnedOn = new_state;
		}

		inline bool get_state(sol::this_state s)
		{
			return turnedOn;
		}

	protected:
		void RenderWindow() override;
		void ProcessEvent(Network::Event);
		void OnChangeNetwork(Network*, Network*) override;

	private:
		bool turnedOn = false;
	};
}