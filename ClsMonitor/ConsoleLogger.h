#pragma once
#include <string>
class ConsoleLogger
{
public:
	// When used in multi-thread context, Require lock fixed
	static ConsoleLogger *getInstance();

	void log(std::string msg);

private:
	static ConsoleLogger *_logger;

	ConsoleLogger();
};