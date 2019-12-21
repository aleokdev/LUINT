#include "UID.h"
#include <cstddef>
#include <string>

namespace LUINT
{
	UID UID::generate()
	{
		return UID(mix(UID::last_hash++));
	}

	UID::UID(std::size_t _hash) : hash(_hash)
	{}

	std::string UID::as_string(const char* format)
	{
		char buf[32];
		sprintf_s(buf, 32, format, hash);

		return std::string(buf);
	}

	std::size_t UID::mix(std::size_t n)
	{
		// Crappy number mix function
		const unsigned char first_byte = (0xFF-n) & 0xFF;
		const unsigned char second_byte = n & 0xFF00;
		const unsigned char third_byte = (0xFF-n) & 0xFF0000;

		return first_byte << 8 + second_byte + third_byte << 16;
	}
}
