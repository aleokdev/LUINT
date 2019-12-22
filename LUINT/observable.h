#pragma once
#include <vector>
#include <algorithm>

// LUINT extension library (lel)
namespace lel
{
	template<typename... Args>
	struct observable
	{
		// Adds a function to the observable to call when operator() is executed.
		inline observable& operator+=(void (*func)(Args...))
		{
			observers.emplace_back(func);
		}

		// Removes a function from the observable list so it doesn't get called when operator() is executed
		inline observable& operator-=(void(*func)(Args...))
		{
			observers.erase(std::remove(observers.begin(), observers.end(), func), observers.end());
		}

		void operator()(Args... args)
		{
			for(auto& observer : observers)
				observer(args)
		}

		std::vector<void(*)(Args...)> observers;
	};
}