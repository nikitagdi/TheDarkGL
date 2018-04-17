#pragma once
#include <string>
#include <map>
#include <boost/lexical_cast.hpp>
//#include "Game.h"




#undef GetMessage
namespace TDEngine
{
	class GameManager;
	extern GameManager GameEngine;
	class MessageManager
	{
	public:
		MessageManager(void);
		~MessageManager(void);

		bool LoadMessagesFile(std::string File);

		std::string GetMessage(std::string message, std::string Default = "");

		bool LoadSettingsFile(std::string File);

		template <typename T>
		T GetSetting(std::string Setting, std::string Default = "")
		{
			using boost::lexical_cast;
			try
			{
				return lexical_cast<T>(Settings.at(Setting));
			}
			catch(std::out_of_range&){GameEngine.ReportError(GetMessage("FAILED_TO_GET_SETTING_NOT_FOUND"));}
			catch(boost::bad_lexical_cast&){GameEngine.ReportError(GetMessage("FAILED_TO_GET_SETTING_BAD_CAST"));}
			try
			{
			if(Default != "")
				return lexical_cast<T>(Default);
			return lexical_cast<T>(Setting);
			}catch(boost::bad_lexical_cast&){GameEngine.ReportError(GetMessage("FAILED_TO_GET_SETTING_BAD_CAST"));}
		}

		void SetSetting(std::string, std::string);

	private:
		std::map<std::string, std::string> Messages;
		std::map<std::string, std::string> Settings;
	};
}
#define GetMessage GetMessageW
