#pragma once
#include "../stdafx.h"
#include <string>
#include <unordered_set>

class Blacklist
{
	HANDLE mutex;
	std::unordered_set<std::wstring> blist;
public:
	Blacklist(std::initializer_list<std::wstring> list);
	Blacklist(std::vector<std::wstring> v);
	Blacklist(const Blacklist& blist);
	// return if not in list
	virtual bool Add(const std::wstring Name);
	// return if in list
	virtual bool Del(const std::wstring Name);
	virtual bool Query(const std::wstring& Name) const;
	virtual ~Blacklist();
	std::unordered_set<std::wstring> CopyBlist() const;
	void SetToBlist(const std::unordered_set<std::wstring> &tolist);
};

