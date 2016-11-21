#include "stdafx.h"
#include "Blacklist.h"
using namespace std;

Blacklist::Blacklist(initializer_list<string> Ilist):blist(Ilist)
{
	mutex = CreateMutex(NULL, FALSE, NULL);
}

bool Blacklist::Add(const std::string Name)
{
	WaitForSingleObject(mutex, INFINITE);
	bool r = blist.insert(Name).second;
	ReleaseMutex(mutex);
	return r;
}

bool Blacklist::Del(const std::string Name)
{
	WaitForSingleObject(mutex, INFINITE);
	bool r = blist.erase(Name);
	ReleaseMutex(mutex);
	return r;
}

bool Blacklist::Query(const std::string Name) const
{
	WaitForSingleObject(mutex, INFINITE);
	bool r = blist.find(Name) != blist.end();
	ReleaseMutex(mutex);
	return false;
}


Blacklist::~Blacklist()
{
	CloseHandle(mutex);
}
