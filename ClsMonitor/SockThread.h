#pragma once
#include "EventQueue.h"

class SockThread
{
	DWORD threadid;
	HANDLE thread;
public:
	const char* ip;
	int port;
	EventQueue *q;
	SockThread(const char* ip, int port, EventQueue *q);
	~SockThread();
};

