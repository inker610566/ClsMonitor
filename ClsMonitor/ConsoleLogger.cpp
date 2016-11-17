#include "stdafx.h"
#include "ConsoleLogger.h"
#include <iostream>
#include <string>
#include <cstdio>
using std::string;
using std::cout;
using std::endl;

ConsoleLogger * ConsoleLogger::_logger = nullptr;

ConsoleLogger * ConsoleLogger::getInstance()
{
	if (_logger == nullptr)
		_logger = new ConsoleLogger();
	return _logger;
}

ConsoleLogger::ConsoleLogger()
{
	if (AllocConsole() == 0)
	{
		// Fatal Error
		exit(GetLastError());
	}
	FILE *f;
	freopen_s(&f, "CONOUT$", "w", stdout);
	SetConsoleTitle(L"Fatal Error Console");
}

void ConsoleLogger::log(string msg)
{
	cout << msg << endl;
}
