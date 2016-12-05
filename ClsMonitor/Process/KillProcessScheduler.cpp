#include "KillProcessScheduler.h"
#include "../Utils/ConsoleLogger.h"

namespace Process
{
	const wchar_t *ScreenLockerPath = L"C:\\Program Files (x86)\\ClsMonitor\\ScreenLocker.exe";
	Process::KillProcessScheduler::KillProcessScheduler(Blacklist *init_list, WMIServiceProxy * service)
		:list(init_list), service(service), init_set(init_list->CopyBlist()), IsScreenLockerStart(false)
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

	void KillProcessScheduler::LockScreen()
	{
		queue.Push(new QueueEvent(Process::LockScreen));
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
			case Process::LockScreen:
				if (!IsScreenLockerStart)
				{
					DoLockScreen();
				}
				break;
			}
			delete evt;
		}

	}

	DWORD WINAPI Wait4Thread(
		_In_ LPVOID lpParameter
	)
	{
		KillProcessScheduler *kpsch = (KillProcessScheduler*)lpParameter;
		STARTUPINFO info={sizeof(info)};
		PROCESS_INFORMATION processInfo;
		if (CreateProcessW(
			ScreenLockerPath,
			NULL,
			NULL,
			NULL,
			FALSE,
			0,
			NULL,
			NULL,
			&info,
			&processInfo))
		{
			WaitForSingleObject(processInfo.hProcess, INFINITE);
			CloseHandle(processInfo.hProcess);
			CloseHandle(processInfo.hThread);
		}
		else
		{
			ConsoleLogger::getInstance()->log("Cannot start ScreenLocker");
		}
		kpsch->IsScreenLockerStart = false;
		return 0;
	}

	void KillProcessScheduler::DoLockScreen()
	{
		IsScreenLockerStart = true;

		DWORD threadid;
		HANDLE thread = CreateThread(
			NULL,
			0,
			Wait4Thread,
			this,
			0,
			&threadid
		);
	}
}
