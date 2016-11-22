#include "stdafx.h"
#include "Blacklist.h"
using namespace std;

Blacklist::Blacklist(initializer_list<wstring> Ilist):blist(Ilist)
{
	mutex = CreateMutex(NULL, FALSE, NULL);
}

bool Blacklist::Add(const std::wstring Name)
{
	WaitForSingleObject(mutex, INFINITE);
	bool r = blist.insert(Name).second;
	ReleaseMutex(mutex);
	return r;
}

bool Blacklist::Del(const std::wstring Name)
{
	WaitForSingleObject(mutex, INFINITE);
	bool r = blist.erase(Name);
	ReleaseMutex(mutex);
	return r;
}

bool Blacklist::Query(const std::wstring Name) const
{
	WaitForSingleObject(mutex, INFINITE);
	bool r = blist.find(Name) != blist.end();
	ReleaseMutex(mutex);
	return r;
}


Blacklist::~Blacklist()
{
	CloseHandle(mutex);
}
