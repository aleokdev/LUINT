#pragma once
#include <string>
#include <cstddef>

namespace LUINT
{
	struct UID;

	struct UID
	{
		static UID generate();

		std::string as_string(const char* format = "{%08x}");

		UID() = delete;
		~UID() = default;

	private:
		UID(std::size_t _hash);

		static std::size_t mix(std::size_t);

		// The number equivalent of this UID.
		std::size_t hash;
		
		// The last hash generated.
		static inline std::size_t last_hash = 0;
	};
}