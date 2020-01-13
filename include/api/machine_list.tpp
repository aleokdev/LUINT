#pragma once
#include "api/machine.hpp"
#include <iostream>
#include <type_traits>

// Template magic begins here

namespace LUINT::Machines::List
{
	struct encapsulated_test
	{
		template<typename MachineType>
		static void callback()
		{
			std::cout << MachineType::static_info.name;
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

		template<typename MachineType>
		int get_type_index()
		{
			return index_next<0, MachineType, MachineTypes...>();
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

		template<
			int index,
			typename MachineTypeToSearchFor,
			typename cur_MachineType,
			typename next_MachineType,
			typename... TypesLeft>
		inline int index_next()
		{
			if (std::is_same <MachineTypeToSearchFor, cur_MachineType>::value)
			{
				return index;
			}
			else
			{
				return index_next<index + 1, MachineTypeToSearchFor, next_MachineType, TypesLeft...>();
			}
		}

		template<
			int index,
			typename MachineTypeToSearchFor,
			typename cur_MachineType>
		inline int index_next()
		{
			if (std::is_same <MachineTypeToSearchFor, cur_MachineType>::value)
			{
				return index;
			}
			else
			{
				return -1; // Not found
			}
		}
	};
}