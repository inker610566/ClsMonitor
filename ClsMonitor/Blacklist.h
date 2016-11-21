#pragma once
#include <string>
#include <unordered_set>

class Blacklist
{
public:
	Blacklist(std::initializer_list<std::string> list);
	// return if not in list
	bool Add(const std::string Name);
	// return if in list
	bool Del(const std::string Name);
	bool Query(const std::string Name) const;
	~Blacklist();
private:
	HANDLE mutex;
	std::unordered_set<std::string> blist;
};

