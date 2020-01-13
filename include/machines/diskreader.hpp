#pragma once
#include "machine.h"

namespace LUINT::Storage
{
	struct GenericStorage
	{
		// TODO
	};
}

namespace LUINT::Machines
{
	struct DiskReader : public Machine
	{
		DiskReader(std::string _name, std::string _manufacturer);

	protected:
		LUINT::Storage::GenericStorage storage;
	};
}