#pragma once
#include <map>
#include <vector>
#include <string>
#include <deque>


namespace TDEngine
{
	// Types for void pointers
	enum TYPE {NONE = -1, BOOL = 0, INT, FLOAT, STRING, CODE, WCHAR};

	class Console
	{
		
	public:
		Console(void);
		~Console(void);
		friend class GameManager;
		struct command
		{
			TYPE type;
			void *p;
			std::wstring description;
			command(TYPE t, void* mem, std::wstring desc = L"") : type(t), p(mem), description(desc) {}
			command(){type = NONE;}
		};

		void RenderConsole();
		void RegisterCommand(std::wstring, TYPE, void*, std::wstring = L"");
		std::wstring HistoryUp();
		std::wstring HistoryDown();
		std::wstring HandleCommand(std::wstring);

		void EnterText(wchar_t);
		void EnterText(std::wstring);
		void DeleteSymbol();
		// 0 is left, 1 is right
		void MoveCarriage(bool);
	
		std::wstring CurrentText;
		int ComNumInHistory;
		int HistoryPos;

	private:
		std::deque<std::wstring> history;
		std::deque<std::wstring> CommandHistory;
		std::map<std::wstring, command> commands;

		int CarriagePos;

		static std::wstring SetWindowSize(std::wstring);
		static std::wstring UpdateWindow(std::wstring);
		static std::wstring SetCharacterSize(std::wstring);
		static std::wstring SendEvent(std::wstring);
		static std::wstring Help(std::wstring);
	};
}