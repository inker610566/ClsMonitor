#pragma once
#include <string>
#include <unordered_set>

class Blacklist
{
public:
	Blacklist(std::initializer_list<std::wstring> list);
	Blacklist(std::vector<std::wstring> v);
	Blacklist(const Blacklist& blist);
	// return if not in list
	virtual bool Add(const std::wstring Name);
	// return if in list
	virtual bool Del(const std::wstring Name);
	virtual bool Query(const std::wstring Name) const;
	virtual ~Blacklist();
	std::unordered_set<std::wstring> CopyBlist();
protected:
	HANDLE mutex;
	std::unordered_set<std::wstring> blist;
};

