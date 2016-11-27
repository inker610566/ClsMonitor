#pragma once
#include <string>
#include <unordered_set>

class Blacklist
{
public:
	Blacklist(std::initializer_list<std::wstring> list);
	Blacklist(std::vector<std::wstring> v);
	// return if not in list
	bool Add(const std::wstring Name);
	// return if in list
	bool Del(const std::wstring Name);
	bool Query(const std::wstring Name) const;
	~Blacklist();
private:
	HANDLE mutex;
	std::unordered_set<std::wstring> blist;
};

