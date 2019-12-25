#pragma once
#include <string>
#include <cstddef>

namespace LUINT
{
	struct UID;
}

namespace std
{
	template<> struct hash<LUINT::UID>;
}

namespace LUINT
{
	struct UID
	{
		static UID generate();

		std::string as_string(const char* format = "{%08x}") const;

		UID() = delete;
		~UID() = default;

		bool operator==(const UID& other) const
		{
			return hash == other.hash;
		}

		bool operator!=(const UID& other) const
		{
			return hash != other.hash;
		}

	private:
		UID(std::size_t _hash);

		static std::size_t mix(std::size_t);

		friend std::hash<LUINT::UID>;
		// The number equivalent of this UID.
		std::size_t hash;
		
		// The last hash generated.
		static inline std::size_t last_hash = 0;
	};
}

namespace std
{
	template<> struct hash<LUINT::UID>
	{
		std::size_t operator()(LUINT::UID const& uid) const noexcept
		{
			return uid.hash;
		}
	};
}