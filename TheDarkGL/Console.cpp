#include <Windows.h>
#include "Console.h"
#include <cmath>
#include <gl/glew.h>
#include <boost/lexical_cast.hpp>
#include "Game.h"
#include "Utils.h"

extern TDEngine::GameManager TDEngine::GameEngine;

using namespace std;

namespace TDEngine
{
	Console::Console(void)
	{
	CurrentText = L"";
	RegisterCommand(L"UpdateWindow",  CODE, &UpdateWindow);
	RegisterCommand(L"SetWindowSize", CODE, &SetWindowSize, L"Sets window's size to WIDTHxHEIGHT");
	RegisterCommand(L"SetTileSize", CODE, &SetCharacterSize);
	RegisterCommand(L"SendEvent", CODE, &SendEvent);
	RegisterCommand(L"Help", CODE, &Help);
	ComNumInHistory = -1;
	HistoryPos = CarriagePos = 0;
	}


	Console::~Console(void)
	{
	}


	void Console::RenderConsole()
	{
		float LineInterval = 5.f;
		//Console window render
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glColor3ub(192,192,192);
		glBegin(GL_QUADS);
			glVertex3f( 1.0f, 1.0f, -1.f);
			glVertex3f(-1.0f, 1.0f, -1.f);
			glVertex3f(-1.0f, 0.0f, -1.f);
			glVertex3f( 1.0f, 0.0f, -1.f);
		glEnd();
		glPopMatrix();

		sf::Text text;
		text.setFont(TDEngine::GameEngine.GraphicsEngine.GetFont(string("arial")));
		text.setCharacterSize(GameEngine.GetSetting<int>("ConsoleCharSize", "18")); 
		text.setColor(sf::Color::Red);
	
		text.setString(CurrentText);
		text.setPosition(5, 0);
		TDEngine::GameEngine.GraphicsEngine.Window->draw(text);
		text.setColor(sf::Color::Black);

		sf::RectangleShape Carriage(sf::Vector2f(3, GameEngine.GetSetting<int>("ConsoleCharSize", "18")));
		Carriage.setFillColor(sf::Color::Red);
		sf::Vector2f pos = text.findCharacterPos(CarriagePos);
		pos.x += (text.findCharacterPos(CarriagePos+1).x - pos.x);
		sf::RenderStates CarriageRS(sf::Transform().translate(pos));
		TDEngine::GameEngine.GraphicsEngine.Window->draw(Carriage, CarriageRS);

		if(HistoryPos < 0)
			HistoryPos = 0;
		else if(HistoryPos >= history.size())
			HistoryPos = history.size()-1;

		float YOffset = Carriage.getGlobalBounds().height + LineInterval;
		for(int i = HistoryPos; i < history.size(); i++)
		{
			text.setString(history[i]);
			if(YOffset < TDEngine::GameEngine.GraphicsEngine.getHeight()/2)
			{
				text.setPosition(5, YOffset);
				TDEngine::GameEngine.GraphicsEngine.Window->draw(text);
			}
			YOffset += text.getGlobalBounds().height + LineInterval;
		}
	}


	void Console::RegisterCommand(wstring name, TYPE type, void* p, std::wstring desc)
	{
	transform(name.begin(),name.end(),name.begin(),tolower);
	commands.insert(pair<wstring, command>(name, command(type, p, desc)));
	}


	wstring Console::HandleCommand(wstring Command)
	{
	bool flag = false;
	wstring name, par(L""), ret(L"");
	for(wchar_t sym : Command)
	{
		if(sym == ' ')
			flag = true;
		else if(!flag)
			name += sym;
		else
			par += sym;
	}
	transform(name.begin(),name.end(),name.begin(),tolower);
	command cmd(commands[name]);
	try
	{
	switch(cmd.type)
	{
	case BOOL:
		*((bool*)cmd.p) = boost::lexical_cast<bool>(par);
		break;
	case INT:
		*((int*)cmd.p) = boost::lexical_cast<int>(par);
		break;
	case FLOAT:
		*((float*)cmd.p) = boost::lexical_cast<float>(par);
		break;
	case STRING:
		*((wstring*)cmd.p) = par;
		break;
	case CODE:
		ret = ((wstring (*)(wstring))cmd.p)(par);
		break;
	case NONE:
		ret = L"Unknown command";
	} //switch
	} //try
	catch(boost::bad_lexical_cast&)
	{
		ret = L"Bad argument";
	}

	
	CommandHistory.push_front(Command);
	if(ret != L"")
		history.push_front(ret);

	history.push_front(Command);

	CarriagePos = 0;

	return ret;
	}


	void Console::EnterText(wchar_t sym)
	{
		if(CarriagePos == 0 || CarriagePos == CurrentText.size())
			CurrentText.insert(CarriagePos, 1, sym);
		else
			CurrentText.insert(CarriagePos+1, 1, sym);
		CarriagePos += 1;
	}


	void Console::EnterText(wstring txt)
	{
		if(CarriagePos == 0 || CarriagePos == CurrentText.size())
			CurrentText.insert(CarriagePos, txt);
		else
			CurrentText.insert(CarriagePos+1, txt);
		CarriagePos += (int)txt.size();
	}


	void Console::DeleteSymbol()
	{
		if(CurrentText.size() > 0)
		{
		//	if(CarriagePos == CurrentText.size())
		//		CurrentText = CurrentText.substr(0, CarriagePos-1);
		//	else
			CurrentText.erase(CarriagePos,1);
			CarriagePos -= 1;
		}
	}

	void Console::MoveCarriage(bool dir)
	{
		int x[] = {-1, 1};
		CarriagePos += x[dir];
		if(CarriagePos == (CurrentText.size()+1))
			CarriagePos = CurrentText.size()-1;
		else if(CarriagePos == -1)
			CarriagePos = 0;
	}

	// Консольные функции
	
	wstring Console::UpdateWindow(wstring par)
	{
		return L"";
	}

	wstring Console::SetWindowSize(wstring par)
	{
	try
	{
	int width, height;
	wstring tmp(L"");
	for(char ch : par)
	{
		if(ch == ':' || ch == 'x' || ch == ',')
		{
			width = boost::lexical_cast<int>(tmp);
			tmp = L"";
		}
		else
			tmp += ch;
	}
	height = boost::lexical_cast<int>(tmp);

	TDEngine::GameEngine.GraphicsEngine.UpdateOpenGL(sf::VideoMode(width, height), "The Dark!", TDEngine::GameEngine.IsFullscreen);

	} catch(boost::bad_lexical_cast&)
	{return L"Incorrect parameters";}

	return L"";
	}

	wstring Console::SetCharacterSize(wstring par)
	{
		try
		{
			boost::lexical_cast<int>(par);
			GameEngine.SetSetting("TileSize", Utils::WstringToString(par));
		} catch(boost::bad_lexical_cast&)
		{return L"Incorrect parameters";}

		return L"";
	}

	wstring Console::SendEvent(wstring par)
	{
		try
		{
			wstring eventName;
			std::vector<wstring> parameters;
			wstring tmp(L"");
			bool flag = false; // if true start reading parameters
			// parse command
			for(wchar_t ch : par)
			{
				if(ch == ' ' || ch == ',')
				{
					if(!flag)
					{
						eventName = tmp;
						tmp = L"";
						flag = true;
					}
					else
					{
						parameters.push_back(tmp);
						tmp = L"";
					}
				}
				else
					tmp += ch;
			}
			parameters.push_back(tmp);
			Event event = GameEngine.EventEngine.GetEventPattern(Utils::WstringToString(eventName));

			if(event.name == "")
				return L"Event" + eventName + L" doesnt exist";			

			if(event.Parameters.size() != parameters.size())
				return L"Incorrect parameters";

			void* ptr = 0;
			for(int i = 0; i < event.Parameters.size(); i++)
			{
				ptr = Utils::CastStringToPointer(event.Parameters[i].first, parameters[i]);
				event.Parameters[i].second = ptr;
			}

			//MessageBoxW(NULL, (*(wstring*)event.Parameters[0].second).c_str(), L"", NULL);

			GameEngine.EventEngine.SendEvent(event);
			
			//Clean up pointers
			for(auto parameter : event.Parameters)
				Utils::DeletePtr(parameter.first, parameter.second);

		} catch(boost::bad_lexical_cast&)
		{return L"Incorrect parameters";}

		return L"";
	}


	std::wstring Console::Help(std::wstring par)
	{
		Console& con = GameEngine.console;
		std::wstring ret = L"";

		if(par == L"")
		{
			std::vector<wstring> lines;
			ret += (wstring(L"Total ") + boost::lexical_cast<wstring>((int)con.commands.size()) + wstring(L" commands: "));
			
			sf::Text text;
			text.setFont(TDEngine::GameEngine.GraphicsEngine.GetFont(string("arial")));
			text.setCharacterSize(GameEngine.GetSetting<int>("ConsoleCharSize", "18")); 
			text.setString(ret);

			sf::Text txt(text);

			for(auto command : con.commands)
			{
				txt.setString(command.first);
				int textSize = (text.getGlobalBounds().width - text.getGlobalBounds().left)
					+ (txt.getGlobalBounds().width - txt.getGlobalBounds().left);
				if((textSize) > (GameEngine.GraphicsEngine.getWidth()))
				{
					lines.push_back(ret);
					ret = L"";
					text.setString(ret);
				}
				ret += command.first + L" ";
				text.setString(ret);
			}
			if(ret != L"")
				lines.push_back(ret);

			lines.push_back(L"Use Help <command> for detailed description");
			ret = L"";

			for(auto str : lines)
				ret += str + boost::lexical_cast<wstring>('\n');
		}

		else
		{
			try
			{
				wchar_t* types[] = {L"BOOL", L"INT", L"FLOAT", L"STRING", L"FUNCTION", L"WCHAR"};
				command c = con.commands.at(par);
				if(c.type == NONE)
					return L"Command not initialized";
				ret += L"Type: " + wstring(types[c.type]);
				if(c.type != CODE)
					ret += L"\nValue: " + Utils::CastPointerToWstring(c.type, c.p);
				if(c.description != L"")
					ret += L"\n"+c.description;
			}
			catch(std::out_of_range&)
			{
				ret = L"Unknown command";
			}
		}

		return ret;
	}

	wstring Console::HistoryUp()
	{
	if((ComNumInHistory-1) <= -1)
		return L"";
	if(ComNumInHistory-1 > -2) 
		ComNumInHistory--;
	return CommandHistory[ComNumInHistory];
	}


	wstring Console::HistoryDown()
	{
	if(CommandHistory.size() == 0)
		return L"";
	if((ComNumInHistory+1) >= CommandHistory.size())
		return CommandHistory[CommandHistory.size()-1];
	if(CommandHistory.size()-ComNumInHistory > 0) 
		ComNumInHistory++;
	return CommandHistory[ComNumInHistory];
	}	


	
}