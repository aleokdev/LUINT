#pragma once
#include "machine.h"

namespace LUINT::Machines
{
	struct LED : public Machine
	{
		LED(LUINT::Data::SessionData& _session, std::string _name, Network* _network) : Machine(_session, _name, _network) {};

		GENERATE_MACHINEINFO(LED, (MachineInfo{ "LED", "aleok studios", "Simple machine that can turn on or off.", Interfaces::get_SimpleOutputDevice() }));

		void ImplementLua(lua_State* state, sol::table&& proxy_table) override;

		struct lua_impl
		{
			void do_thing()
			{
				std::cout << "thing";
			}
		};

		inline void f_set_state(bool new_state)
		{
			turnedOn = new_state;
		}

		inline bool f_get_state()
		{
			return turnedOn;
		}

	protected:
		void RenderWindow() override;

	private:
		bool turnedOn = false;
	};
}