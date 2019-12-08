#pragma once
#include <string>
#include <cstddef>

namespace LUINT
{
	struct UID;

	struct UID
	{
		static UID generate();

		std::string as_string();

		UID() = delete;
		~UID() = default;

	private:
		UID(std::size_t _hash);

		// The number equivalent of this UID.
		std::size_t hash;
		
		// The last hash generated.
		static inline std::size_t last_hash = 0;
	};
}