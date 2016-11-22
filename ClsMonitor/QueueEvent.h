#pragma once

#include<comdef.h>
#include<string>

enum EventType
{
	Add, Del, Kill
};

class QueueEvent
{
public:
	QueueEvent(bool AddOrDel, std::wstring Name);
	QueueEvent(VARIANT RelPath);
	~QueueEvent();
	union {
		VARIANT RelPath;
		std::wstring Name;
	};
	EventType type;
};

