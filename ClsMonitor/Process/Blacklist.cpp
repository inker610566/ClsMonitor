#include "../stdafx.h"
#include "Blacklist.h"
using namespace std;

Blacklist::Blacklist(initializer_list<wstring> Ilist):blist(Ilist)
{
	mutex = CreateMutex(NULL, FALSE, NULL);
}

Blacklist::Blacklist(vector<wstring> v):blist(v.begin(), v.end())
{
	mutex = CreateMutex(NULL, FALSE, NULL);
}

Blacklist::Blacklist(const Blacklist& blist)
	:blist(blist.blist)
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

bool Blacklist::Query(const std::wstring& Name) const
{
	WaitForSingleObject(mutex, INFINITE);
	bool r = blist.find(Name) != blist.end();
	ReleaseMutex(mutex);
	return r;
}

std::unordered_set<std::wstring> Blacklist::CopyBlist() const
{
	WaitForSingleObject(mutex, INFINITE);
	std::unordered_set<std::wstring> ret = blist;
	ReleaseMutex(mutex);
	return ret;
}

void Blacklist::SetToBlist(const std::unordered_set<std::wstring>& tolist)
{
	WaitForSingleObject(mutex, INFINITE);
	blist = tolist;
	ReleaseMutex(mutex);
}

Blacklist::~Blacklist()
{
	CloseHandle(mutex);
}

