#pragma once
#include <string>
#include <fstream>

namespace TDEngine
{
	class Logger
	{
	public:
		enum COLORS { BLACK, DARKBLUE, GREEN, DARKTEAL, DARKRED, PURPLE,
			DRKYELLOW, LIGHTGRAY, GRAY, BLUE, LIGHTGREEN, TEAL, RED, PINK, 
			YELLOW, WHITE};

		Logger();

		bool Init(std::string filename); //filename to be logged

		void LogError(std::string text, std::string module = "");
		void LogInfo(std::string text, std::string module = "");
		void LogWarning(std::string text, std::string module = "");

		void SetWriteToFile(bool flag);

	private:
		std::string Filename;
		bool isNeedFileLog; // write log to file if true
		std::ofstream File;
		void* ConsoleHandle;
	};
}