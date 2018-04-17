#include "Logger.h"
#include <Windows.h>
#include <iostream>
#include <ctime>
#include "Utils.h"

namespace TDEngine
{
	Logger::Logger()
	{

	}


	bool Logger::Init(std::string filename)
	{
		Filename = filename;
		ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		
		File.open(Filename, std::ios::app);
		if(!File.is_open())
		{
			LogError("Failed to create new log file "+Filename + ", shutting down file logging", "Logger");
			SetWriteToFile(false);
			return true;
		}
		SetWriteToFile(true);

		return true;
	}


	void Logger::SetWriteToFile(bool flag)
	{
		isNeedFileLog = true;
	}


	void Logger::LogWarning(std::string text, std::string module)
	{
		SetConsoleTextAttribute(ConsoleHandle, YELLOW + 16*BLACK);
		std::cout << Utils::GetCurrentTimeString()+"[Warning] ";

		SetConsoleTextAttribute(ConsoleHandle, WHITE + 16*BLACK);
		std::cout << module + " - " + text << std::endl;

		if(isNeedFileLog)
			File << Utils::GetCurrentTimeString()+"[Warning] " + module + " - " + text << std::endl;
	}


	void Logger::LogError(std::string text, std::string module)
	{
		SetConsoleTextAttribute(ConsoleHandle, RED + 16*BLACK);
		std::cout << Utils::GetCurrentTimeString()+"[Error] ";

		SetConsoleTextAttribute(ConsoleHandle, WHITE + 16*BLACK);
		std::cout << module + " - " + text << std::endl;

		if(isNeedFileLog)
			File << Utils::GetCurrentTimeString()+"[Error] " + module + " - " + text << std::endl;
	}


	void Logger::LogInfo(std::string text, std::string module)
	{
		std::cout << Utils::GetCurrentTimeString()+"[Info] ";
		std::cout << module + " - " + text << std::endl;

		if(isNeedFileLog)
			File << Utils::GetCurrentTimeString()+"[Info] " + module + " - " + text << std::endl;
	}
}