#pragma once
#include "machine.h"
#include <iostream>

// Template magic begins here

namespace LUINT::Machines::List
{
	struct encapsulated_test
	{
		template<typename MachineType>
		static void callback()
		{
			std::cout << MachineType::static_name;
		}
	};

	// The MachineList struct takes a list of machine types. Once the list is created, eval_for_each can be called with an encapsulated void callback() function,
	// which takes one machine type as a template argument. This function is called for every machine present in the machine list.
	// For an example of how the encapsulated function works, take a look at encapsulated_test::callback().
	template<typename... MachineTypes>
	struct MachineList
	{
		template<typename EncapsulatedCallback>
		void eval_for_each()
		{
			eval_next<EncapsulatedCallback, MachineTypes...>();
		}

	private:
		// The only reason FollowingType is here is because C++ allows empty template parameter lists, and so it generates a compiler error when there's only
		// one item in the function because it confuses it with the other eval_next.
		template<
			typename EncapsulatedCallback,
			typename NextType,
			typename FollowingType,
			typename... TypesLeft>
		inline void eval_next()
		{
			EncapsulatedCallback::template callback<NextType>();

			eval_next<EncapsulatedCallback, FollowingType, TypesLeft...>();
		}

		template<
			typename EncapsulatedCallback,
			typename FinalType>
		inline void eval_next()
		{
			EncapsulatedCallback::template callback<FinalType>();
		}
	};
}