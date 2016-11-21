#include "stdafx.h"
#include "EventQueue.h"


EventQueue::EventQueue()
{
	pending_pop = CreateEvent(
		NULL,
		FALSE,
		FALSE,
		NULL
	);
	InitializeCriticalSection(&sec);
}

QueueEvent * EventQueue::Pop()
{
	QueueEvent *evt = nullptr;
	EnterCriticalSection(&sec);
	if (!q.empty())
	{
		evt = q.back();
		q.pop();
	}
	LeaveCriticalSection(&sec);
	if(evt == nullptr)
		WaitForSingleObject(pending_pop, INFINITE);
	return evt;
}


void EventQueue::Push(QueueEvent * evt)
{
	EnterCriticalSection(&sec);
	q.push(evt);
	if (q.size() == 1)
		SetEvent(pending_pop);
	LeaveCriticalSection(&sec);
}

EventQueue::~EventQueue()
{
	DeleteCriticalSection(&sec);
	CloseHandle(pending_pop);
}
