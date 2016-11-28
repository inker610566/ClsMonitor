#pragma once
#include<queue>
#include "QueueEvent.h"

namespace Process
{
	// single consumer, mutiple producer
	class EventQueue
	{
	public:
		EventQueue();
		QueueEvent *Pop();
		void Push(QueueEvent *evt);
		~EventQueue();
	private:
		std::queue<QueueEvent*> q;
		CRITICAL_SECTION sec;
		HANDLE pending_pop;
	};
}
