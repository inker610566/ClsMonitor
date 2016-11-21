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
	QueueEvent(bool AddOrDel, std::string Name);
	QueueEvent(VARIANT RelPath);
	~QueueEvent();
	union {
		VARIANT RelPath;
		std::string Name;
	};
	EventType type;
};

