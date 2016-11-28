#include "../stdafx.h"
#include "Frame.h"

Network::Frame::Frame(unsigned char type, std::wstring && param)
	:type(type), param(param)
{
}
