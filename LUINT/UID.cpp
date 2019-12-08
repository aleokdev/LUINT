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
		std::string result = std::string();
		result.append("{");
		result.append(std::to_string((unsigned int)UID::last_hash));
		result.append("}");

		return result;
	}
}
