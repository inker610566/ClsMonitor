#pragma once

#include<comdef.h>
#include<string>

namespace Process
{
	enum EventType
	{
		Add, Del, Kill, Reset
	};

	class QueueEvent
	{
	public:
		QueueEvent(EventType type, std::wstring Name);
		QueueEvent(EventType type);
		~QueueEvent();
		std::wstring Name;
		EventType type;
	};
}
