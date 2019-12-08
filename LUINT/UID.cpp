#include "UID.h"
#include <cstddef>
#include <string>

namespace LUINT
{
	UID UID::generate()
	{
		return UID(UID::last_hash++);
	}

	UID::UID(std::size_t _hash) : hash(_hash)
	{}

	std::string UID::as_string()
	{
		char buf[32];
		sprintf_s(buf, 32, "{%08i}", hash);

		return std::string(buf);
	}
}
