#include "stdafx.h"
#include "QueueEvent.h"

QueueEvent::QueueEvent(bool AddOrDel, std::wstring Name):Name(Name)
{
	type = AddOrDel ? Add : Del;
}

QueueEvent::QueueEvent(VARIANT RelPath):RelPath(RelPath)
{
	type = Kill;
}

QueueEvent::~QueueEvent()
{
	if (type == Kill)
		VariantClear(&RelPath);
}
