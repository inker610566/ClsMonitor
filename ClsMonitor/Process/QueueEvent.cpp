#include "../stdafx.h"
#include "QueueEvent.h"

QueueEvent::QueueEvent(bool AddOrDel, std::wstring Name):Name(Name)
{
	type = AddOrDel ? Add : Del;
}

QueueEvent::QueueEvent(VARIANT Name):Name(Name.bstrVal, SysStringLen(Name.bstrVal))
{
	type = Kill;
}

QueueEvent::~QueueEvent()
{
}
