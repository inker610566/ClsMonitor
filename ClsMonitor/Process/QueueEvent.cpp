#include "../stdafx.h"
#include "QueueEvent.h"

namespace Process
{
	QueueEvent::QueueEvent(EventType type, std::wstring Name)
		: type(type), Name(Name)
	{
	}

	QueueEvent::QueueEvent(EventType type)
		: type(type)
	{
	}

	QueueEvent::~QueueEvent()
	{
	}
}
