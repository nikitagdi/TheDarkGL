#include "MessageManager.h"
#include <fstream>
#include <string>
#include "INI1.31.h"

#undef GetMessage

using namespace std;


TDEngine::MessageManager::MessageManager(void)
{
}


TDEngine::MessageManager::~MessageManager(void)
{
}


bool TDEngine::MessageManager::LoadMessagesFile(std::string File)
{
	std::ifstream file(File);
	if(file.fail())
		return false;
	std::string key, data, line;
	bool dataFlag = 0; //Если 1, то читать строку в data
	
	while(file.good())
	{
		dataFlag = 0;
		key = data = "";
		std::getline(file, line);
		for(char sym : line)
		{
			if(sym == '#' || sym == '\n')
				break;
			else if(sym == '=')
			{
				dataFlag = 1;
				continue;
			}
			if(dataFlag)
				data+=sym;
			else
				key+=sym;
		}
		if(data != "" && key != "")
			Messages[key]=data;
	}
	file.close();
	return true;
}


std::string TDEngine::MessageManager::GetMessage(std::string message, std::string Default)
{
	try
	{
		return Messages.at(message);
	}
	catch(std::out_of_range&)
	{
		if(Default != "")
			return Default;
		return message;
	}
}


bool TDEngine::MessageManager::LoadSettingsFile(std::string File)
{
	std::ifstream file(File);
	if(file.fail())
		return false;
	file.close();

	typedef INI <string, string, string> ini_t;
	ini_t ini(File, true);

	for(auto i = ini.sections.begin(); i != ini.sections.end(); i++)
		   for(auto j = i->second->begin(); j != i->second->end(); j++)
			  Settings[j->first] = j->second;
}


void TDEngine::MessageManager::SetSetting(std::string setting, std::string value)
{
	Settings[setting] = value;
}

