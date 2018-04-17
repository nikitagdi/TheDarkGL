#pragma once
#include "EventManager.h"
#include "GraphicsManager.h"
#include "GameState.h"
#include "MessageManager.h"
#include "Console.h"
#include <thread>
#include "Logger.h"
#include "Clock.h"


#undef GetMessage
namespace TDEngine
{
	class Console;

	class GameManager
	{
	public:
		GameManager();
		//~GameManager();
		friend class Console;
		bool Init();
		void MainLoop();
		

		void ReportError(std::string message);
		void ReportErrorAndExit(std::string message);

		static GraphicsManager GraphicsEngine;

		GameManager& getSingleton()
		{
			static GameManager* gm;
			if (!gm)
				gm = new GameManager();
			return *gm;
		}

		template<typename T>
		T GetSetting(std::string setting, std::string Default = "")
		{
			return MessageEngine.GetSetting<T>(setting, Default);
		}

		
		void SetSetting(std::string, std::string);

		std::string GetMessage(std::string message, std::string Default = "")
		{
			return MessageEngine.GetMessage(message, Default);
		}

		GameState* GetCurrentGameState() const {return CurrentGameState;}

		float GetFrameInterval()    const {return frameInterval;}
		//Returns time elapsed from last game tick in milliseconds
		//float getElapsedTime()      { return (logicTime + renderTime) > 1000.f / TPS ? (logicTime + renderTime) : 1000.f / TPS; }
		float getElapsedTime() const { return timeElapsed; }
		float getActualTime ()		{ return gameClock.getElapsedTime();}
		//Returns time taken for last logic tick
		float getLogicTime() const { return logicTime; }
		//Returns time taken for last render tick
		float getRenderTime() const { return renderTime; }
		
		bool IsConsoleActivated() const {return IsConsoleActive;}

		EventManager EventEngine;
		
		Console console;
		Logger LogEngine;
        bool IsNeedHandleControls;
		bool IsFocused, IsExitRequired, IsFullscreen, IsConsoleActive ;
		
		
	private:
		void RenderFPS();
		void Render();
		void ProceedDefaultEvent(sf::Event&);
		void InjectCEGUIEvent(sf::Event&);

		sf::Thread* RenderThread;
		GameState* CurrentGameState;
		MessageManager MessageEngine;

		
		bool LockGraphicsThread;
		float frameInterval;
		//Time elapsed from last tick
		float timeElapsed;
		//Time taken by each subsystem
		float logicTime;
		float renderTime;

		float FPS;
		float TPS;
		Clock gameClock;
		Clock renderClock;
	};
}