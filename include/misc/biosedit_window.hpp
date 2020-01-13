#pragma once
#include <string>
#include "api/UID.hpp"

#define MAX_BIOS_SIZE 65536

namespace LUINT
{
	namespace GUI
	{
		struct BIOSEditWindow
		{
			BIOSEditWindow(std::string* b) : bios(b)
			{
				strcpy_s(text_buffer, b->size()+1, b->c_str());
			}

			void Draw(bool* p_open);

		private:
			UID uid = UID::generate();
			std::string* bios;
			char text_buffer[MAX_BIOS_SIZE];
		};
	}
}