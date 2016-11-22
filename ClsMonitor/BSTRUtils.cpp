#include "stdafx.h"
#include "BSTRUtils.h"



namespace BSTRUtils
{
	std::wstring BSTRToWString(const BSTR& bstr)
	{
		return std::wstring(bstr, SysStringLen(bstr));
	}
}