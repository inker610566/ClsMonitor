#pragma once
#include "Blacklist.h"
#include "SockThread.h"

class BlacklistNT :
	public Blacklist
{
private:
	Blacklist *_NoNT;
	SockThread *sock;
public:
	// NoNT should be read only
	BlacklistNT(SockThread *sock, Blacklist *NoNT);

	// return if not in list
	virtual bool Add(const std::wstring Name);
	// return if in list
	virtual bool Del(const std::wstring Name);
	virtual bool Query(const std::wstring Name) const;

	// reset to NoNT list
	// should be called before reconnect to server
	void Reset();

	~BlacklistNT();
};

