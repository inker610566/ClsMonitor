#pragma once

#include "Blacklist.h"
#include "EventQueue.h"
#include "../WMI/WMIServiceProxy.h"
#include <vector>
#include <unordered_set>

namespace Process
{
	class KillProcessScheduler
	{
	private:
		Blacklist *list;
		WMIServiceProxy *service;
		EventQueue queue;
		std::unordered_set<std::wstring> init_set;
		// switch control to lock screen
		// block until unlock
		void DoLockScreen();

	public:
		bool IsScreenLockerStart;
		KillProcessScheduler(Blacklist *list, WMIServiceProxy *service);
		void Add(std::wstring name);
		void Del(std::wstring name);
		void Kill(std::wstring name);
		void LockScreen();
		// reset to init_list;
		void Reset();

		// Require a thread to execute the queue event
		void ExecuteLoop();
	};
}