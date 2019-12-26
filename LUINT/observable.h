#pragma once
#include <vector>
#include <algorithm>
#include <functional>
#include "UID.h"

// LUINT extension library (lel)
namespace lel
{
	using UID = LUINT::UID;

	template<typename... Args>
	struct observable
	{
		// Adds a function to the observable to call when operator() is executed.
		inline void add_observer(UID& uid, std::function<void(Args...)> func)
		{
			observers.insert(std::pair<UID, std::function<void(Args...)>>(uid, func));
		}

		// Removes a function from the observable list so it doesn't get called when operator() is executed
		inline void remove_observer(UID& uid)
		{
			observers.erase(uid);
		}

		void operator()(Args... args)
		{
			for (const std::pair<UID, std::function<void(Args...)>>& pair : observers)
				pair.second(args...);
		}

		std::unordered_map<UID, std::function<void(Args...)>> observers;
	};
}