#include "stdafx.h"
#include "BlacklistNT.h"
using namespace std;

BlacklistNT::BlacklistNT(SockThread *sock, Blacklist *NoNT)
	: sock(sock), Blacklist(*NoNT), _NoNT(NoNT)
{
}

bool BlacklistNT::Add(const std::wstring Name)
{
	if (sock->IsConnected)
		return Blacklist::Add(Name);
	else
		return false;
}

bool BlacklistNT::Del(const std::wstring Name)
{
	if (sock->IsConnected)
		return Blacklist::Del(Name);
	else
		return false;
}

bool BlacklistNT::Query(const std::wstring Name) const
{
	if (sock->IsConnected)
		return Blacklist::Query(Name);
	else
		return _NoNT->Query(Name);
}

void BlacklistNT::Reset()
{
	WaitForSingleObject(mutex, INFINITE);
	blist = _NoNT->CopyBlist();
	ReleaseMutex(mutex);
}

BlacklistNT::~BlacklistNT()
{
	CloseHandle(mutex);
}
