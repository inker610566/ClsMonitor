#include "KillProcessScheduler.h"

namespace Process
{
	Process::KillProcessScheduler::KillProcessScheduler(Blacklist *init_list, WMIServiceProxy * service)
		:list(init_list), service(service), init_set(init_list->CopyBlist())
	{
	}

	void Process::KillProcessScheduler::Add(std::wstring name)
	{
		queue.Push(new QueueEvent(Process::Add, name));
	}

	void Process::KillProcessScheduler::Del(std::wstring name)
	{
		queue.Push(new QueueEvent(Process::Del, name));
	}

	void KillProcessScheduler::Kill(std::wstring name)
	{
		queue.Push(new QueueEvent(Process::Kill, name));
	}

	void Process::KillProcessScheduler::Reset()
	{
		queue.Push(new QueueEvent(Process::Reset));
	}

	void Process::KillProcessScheduler::ExecuteLoop()
	{
		while (true)
		{
			QueueEvent* evt = queue.Pop();
			switch (evt->type)
			{
			case Process::Add:
				list->Add(evt->Name);
				service->TerminateProcessesWithName(evt->Name);
				break;
			case Process::Del:
				list->Del(evt->Name);
				break;
			case Process::Kill:
				service->TerminateProcessesWithName(evt->Name);
				break;
			case Process::Reset:
				list->SetToBlist(init_set);
				for (auto s: init_set)
					service->TerminateProcessesWithName(s);
				break;
			}
			delete evt;
		}

	}
}
