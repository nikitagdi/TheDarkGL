#include "GameStateGame.h"
#include "Game.h"
#include "GameStateInit.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include "Utils.h"
#include "Console.h"

#undef GetMessage

using namespace TDEngine;
using namespace std;

GraphicsManager GameManager::GraphicsEngine;






GameManager::GameManager()
{

	IsExitRequired = 0;
	IsFocused      = 1;
	IsFullscreen   = 0;
	IsNeedHandleControls = 1;
	LogEngine.Init("dasd");
	if(!MessageEngine.LoadSettingsFile("Settings.ini"))
		LogEngine.LogError("Failed to load base settings!");
	if(!MessageEngine.LoadMessagesFile(MessageEngine.GetSetting<string>("Messages")))
		LogEngine.LogError("Failed to load messages!");
}


bool GameManager::Init()
{
	float diff = 0.f;
	FPS=MessageEngine.GetSetting<int>("MaxFPS", "60");
	TPS=MessageEngine.GetSetting<int>("TPS", "30");
	GraphicsEngine.InitGraphics();
	LogEngine.LogInfo("Graphics engine has been initialized.", "Game manager");

	LogEngine.LogInfo("All engine subsystems have been loaded.", "Game manager");

	CurrentGameState = new TDGame::GameStateGame();

//	Utils::ClipCursorWnd(GraphicsEngine.Window);



	return true;
}


void GameManager::InjectCEGUIEvent(sf::Event& event)
{
/*
	switch (event.type)
	{
	case sf::Event::MouseMoved:
		context.injectMousePosition(event.mouseMove.x, event.mouseMove.y);
		break;
	case sf::Event::MouseButtonPressed:
		if(event.mouseButton.button == sf::Mouse::Left)
			context.injectMouseButtonDown(CEGUI::MouseButton::LeftButton);
		else if (event.mouseButton.button == sf::Mouse::Right)
			context.injectMouseButtonDown(CEGUI::MouseButton::RightButton);
		else if (event.mouseButton.button == sf::Mouse::Middle)
			context.injectMouseButtonDown(CEGUI::MouseButton::MiddleButton);
		else
			context.injectMouseButtonDown(CEGUI::MouseButton::NoButton);
		break;
	case sf::Event::MouseButtonReleased:
		if (event.mouseButton.button == sf::Mouse::Left)
			context.injectMouseButtonUp(CEGUI::MouseButton::LeftButton);
		else if (event.mouseButton.button == sf::Mouse::Right)
			context.injectMouseButtonUp(CEGUI::MouseButton::RightButton);
		else if (event.mouseButton.button == sf::Mouse::Middle)
			context.injectMouseButtonUp(CEGUI::MouseButton::MiddleButton);
		else
			context.injectMouseButtonUp(CEGUI::MouseButton::NoButton);
		break;
	case sf::Event::KeyPressed:
		context.injectKeyUp(m_mapSFKeyToCEGUIKey[event.key.code]);
		break;
	case sf::Event::KeyReleased:
		context.injectKeyDown(m_mapSFKeyToCEGUIKey[event.key.code]);
		break;
	case sf::Event::TextEntered:
		context.injectChar(event.text.unicode);
			break;
	default:
		break;
	}*/
}


void GameManager::ProceedDefaultEvent(sf::Event& event)
{
		//if (event.type == sf::Event::Closed){
		//	IsExitRequired = true;exit(1);}

	InjectCEGUIEvent(event);
		
		if(GetFocus() != GraphicsEngine.Window->getSystemHandle())
			IsFocused = false;

		if (event.type == sf::Event::MouseMoved) {}
		else if( event.type == sf::Event::MouseButtonPressed)
		{
			if(!IsFocused)
			{
				IsFocused = true;
				SetFocus(GraphicsEngine.Window->getSystemHandle());
			}
		}
		else if( event.type == sf::Event::MouseButtonReleased)
		{
		}
		else if( event.type == sf::Event::LostFocus)
			IsFocused = false;
		else if( event.type == sf::Event::GainedFocus)
		{
			Utils::ClipCursorWnd(GraphicsEngine.Window);
			IsFocused = true;
		}
		else if (event.type == sf::Event::EventType::MouseWheelMoved)
		{
			TDEngine::Event ev = TDEngine::GameEngine.EventEngine.GetEventPattern("event_mouseWheelTurned");
			ev.Parameters[0].second = &event.mouseWheel.delta;
			TDEngine::GameEngine.EventEngine.SendEvent(ev);
		}
		if(IsFocused)
		{
			if( event.type == sf::Event::TextEntered)
			{
				if(IsConsoleActive)
				{
					IsNeedHandleControls = false;
					if(event.text.unicode == 13)
					{
						if(console.CurrentText != L"")
							console.HandleCommand(console.CurrentText);
						console.CurrentText = L"";
					}
					else if(event.text.unicode == 8)
						console.DeleteSymbol();
					else
						if((event.text.unicode != '`') && (event.text.unicode != 1105) && (event.text.unicode != 22))
							console.EnterText(event.text.unicode);
				}
			}

			if(event.type == sf::Event::KeyPressed)
			{

				TDEngine::Event ev = TDEngine::GameEngine.EventEngine.GetEventPattern("event_buttonPressed");
				ev.Parameters[0].second = &event.key;
				TDEngine::GameEngine.EventEngine.SendEvent(ev);
				if(IsConsoleActive)
				{
					switch(event.key.code)
					{
					case sf::Keyboard::Tilde:
						IsConsoleActive = !IsConsoleActive;
						console.CurrentText = L"";
						console.ComNumInHistory = -1;
						IsNeedHandleControls = true;
						break;
					case sf::Keyboard::Escape:
						IsExitRequired = true;
						break;
					case sf::Keyboard::Up:
						console.CurrentText = console.HistoryUp();
						break;
					case sf::Keyboard::Down:
						console.CurrentText = console.HistoryDown();
						break;
					case sf::Keyboard::Left:
						console.MoveCarriage(0);
						break;
					case sf::Keyboard::Right:
						console.MoveCarriage(1);
						break;
					case sf::Keyboard::V:
						if(event.key.control)
						{
							console.EnterText(Utils::GetTextFromClipboard());
						}
						break;
						
					} //switch(event.key.code)
				} //IsConsoleActivated

				else
					if(IsNeedHandleControls)
						switch(event.key.code)
						{
						case sf::Keyboard::Escape:
							IsExitRequired = true;
							exit(1);
							break;
						case sf::Keyboard::F11:
							IsFullscreen = !IsFullscreen;
							GraphicsEngine.UpdateOpenGL(sf::VideoMode(GraphicsEngine.WinWidth, GraphicsEngine.WinHeight), MessageEngine.GetSetting<std::string>("WindowTitle"), IsFullscreen, MessageEngine.GetSetting<int>("MaxFPS"));
							break;
						case sf::Keyboard::Tilde:
							IsConsoleActive = !IsConsoleActive;
							break;
						case sf::Keyboard::BackSpace:

							break;
						}//switch(event.key.code)
					}//if(event.type == sf::Event::KeyPressed)

			else if (event.type == sf::Event::KeyReleased)
			{
				TDEngine::Event ev = TDEngine::GameEngine.EventEngine.GetEventPattern("event_buttonReleased");
				ev.Parameters[0].second = &event.key;
				TDEngine::GameEngine.EventEngine.SendEvent(ev);
			}
		}
}


void GameManager::MainLoop()
{
	float startTime = 0.f, endTime = 0.f;

	bool isMultithreaded = GetSetting<int>("MultithreadedMode", "0");

         
	double elapsedTime;


	GraphicsEngine.desktop.Update(0.f);

	//gameClock.setMode(Clock::STANDART);

	while(!IsExitRequired)
	{
		float TickTime = 1000.f / TPS;
		float RenderTickTime = 1000.f / FPS;

		sf::Event event;
		
		GameState* NewGameState = CurrentGameState->getNewGameState();
		if(NewGameState)
		{
			IsFocused = false;
			delete CurrentGameState;
			CurrentGameState = NewGameState;
			IsFocused = true;
		}

		//Input processing
		while (GraphicsEngine.Window->pollEvent(event))
		{
			GraphicsEngine.desktop.HandleEvent(event);
			ProceedDefaultEvent(event);
			CurrentGameState->proceedEvent(event);
		} 

		//Logic tick
		if (gameClock.getElapsedTime() >= TickTime-logicTime)
		{
			timeElapsed = gameClock.getElapsedTime();

			gameClock.restart();
			float begin = gameClock.getElapsedTime();
			CurrentGameState->logic();
			float end = gameClock.getElapsedTime();
			logicTime = end - begin;		
		}

		//Render tick
		if (!isMultithreaded && renderClock.getElapsedTime() >= RenderTickTime)
		{
			float begin = renderClock.getElapsedTime();
			GraphicsEngine.Render();
			float end = renderClock.getElapsedTime();
			renderTime = end - begin;
			renderClock.restart();
		}

		if (!RenderThread && isMultithreaded)
		{
			GraphicsEngine.Window->setActive(false);
			RenderThread = new sf::Thread([&]() {while (true) { GraphicsEngine.Render(); }});
			RenderThread->launch();
		}
			
	}
	
}


void GameManager::Render()
{//std::cout << "i";
	GraphicsEngine.Window->clear();

	CurrentGameState->renderScene();

	//Rendering FPS counter and console
	GraphicsEngine.Window->pushGLStates();
	GraphicsEngine.Window->setView(sf::View(sf::FloatRect(0.f,0.f,GraphicsEngine.getWidth(),GraphicsEngine.getHeight())));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if(IsConsoleActive)
		console.RenderConsole();
	if(!IsConsoleActive)
		RenderFPS();

	GraphicsEngine.RenderText("Frame time: "+boost::lexical_cast<std::string>(timeElapsed),
		sf::Vector2f(200.f, 0.f), GraphicsEngine.GetFont("arial"), sf::Color(255.f, 0.f,0.f));

	GraphicsEngine.Window->popGLStates();
	TDEngine::GameEngine.GraphicsEngine.Window->pushGLStates();
	TDEngine::GameEngine.GraphicsEngine.Window->setView(sf::View(sf::FloatRect(0.f, 0.f, TDEngine::GameEngine.GraphicsEngine.getWidth(), TDEngine::GameEngine.GraphicsEngine.getHeight())));
	TDEngine::GameEngine.GraphicsEngine.RenderGUI();
	TDEngine::GameEngine.GraphicsEngine.Window->popGLStates();
	GraphicsEngine.Window->display();
	
}


void GameManager::ReportError(std::string message)
{
	std::cout << message << std::endl;
}


void GameManager::ReportErrorAndExit(std::string message)
{
	std::cout << message << std::endl;
	exit(1);
}


void GameManager::RenderFPS()
{
	static float fpst = 0, prevTicks = 0, fps = 0, frameTime = 0;
	static sf::Clock clock;
	float currentTime = clock.getElapsedTime().asSeconds();

	frameInterval = currentTime - frameTime;
	frameTime = currentTime;

	fpst++;
	if(currentTime-prevTicks > 1.0f)
	{
		prevTicks=currentTime;
		fps=fpst;
		fpst=0;
	}
	sf::Text text("FPS: "+boost::lexical_cast<std::string>((int)fps), GraphicsEngine.GetFont("arial"));
	text.setColor(sf::Color(255,0,0));
	GraphicsEngine.Window->draw(text);
	
}


void GameManager::SetSetting(std::string setting, std::string value)
{
	MessageEngine.SetSetting(setting, value);
}
