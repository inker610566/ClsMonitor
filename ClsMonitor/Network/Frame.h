#pragma once

#include <string>

namespace Network
{
	struct Frame
	{
		unsigned char type;
		std::wstring param;
		Frame(unsigned char type, std::wstring&& param);
	};
}