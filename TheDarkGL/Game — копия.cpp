#include "GameStateGame.h"
#include "Game.h"
#include "GameStateInit.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include "Utils.h"
#include "Console.h"
#include <CEGUI/System.h>
#include <CEGUI/GUIContext.h>
#include <CEGUI/widgets/PushButton.h>
#include <CEGUI/RendererModules/OpenGL/GLRenderer.h>

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
	GraphicsEngine.SetWindowParams(MessageEngine.GetSetting<int>("Width"), MessageEngine.GetSetting<int>("Height"),
		MessageEngine.GetSetting<int>("MaxFPS"), MessageEngine.GetSetting<int>("FullscreenMode"), MessageEngine.GetSetting<string>("WindowTitle"));
	GraphicsEngine.UpdateWindow();
	GraphicsEngine.LoadFonts(MessageEngine.GetSetting<std::string>("Fonts"));

	WidgetRenderer = &CEGUI::OpenGLRenderer::create();
	CEGUI::System::create(*WidgetRenderer);

	TDEngine::GameEngine.GraphicsEngine.Window->pushGLStates();
GraphicsEngine.LoadTextures(MessageEngine.GetSetting<std::string>("Textures"));
	TDEngine::GameEngine.GraphicsEngine.Window->popGLStates();
	CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>
    (CEGUI::System::getSingleton().getResourceProvider());
	rp->setResourceGroupDirectory("schemes", "Data/GUI/schemes/");
	rp->setResourceGroupDirectory("imagesets", "Data/GUI/imagesets/");
	rp->setResourceGroupDirectory("fonts", "Data/GUI/fonts/");
	rp->setResourceGroupDirectory("layouts", "Data/GUI/layouts/");
	rp->setResourceGroupDirectory("looknfeels", "Data/GUI/looknfeel/");
	rp->setResourceGroupDirectory("lua_scripts", "Data/GUI/lua_scripts/");
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");
	CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");

	CEGUI::SchemeManager::getSingleton().createFromFile( TDEngine::GameEngine.GetSetting<std::string>("DefaultWidgetLook")+".scheme" ); 
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow( CEGUI::WindowManager::getSingleton().createWindow( "DefaultWindow", "_MasterRoot" ) );
	DefaultContext = &CEGUI::System::getSingleton().getDefaultGUIContext();


	

	CurrentGameState = new TDGame::GameStateGame();

//	CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont( "DejaVuSans-10" );
//	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage( "TaharezLook/MouseArrow" );
//	CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultTooltipType( "TaharezLook/Tooltip" );


//	Utils::ClipCursorWnd(GraphicsEngine.Window);

	TPS=MessageEngine.GetSetting<int>("TPS","50");

	return true;
}


void GameManager::ProceedDefaultEvent(sf::Event& event)
{
		//if (event.type == sf::Event::Closed){
		//	IsExitRequired = true;exit(1);}

		

		if(GetFocus() != GraphicsEngine.Window->getSystemHandle())
			IsFocused = false;

		if( event.type == sf::Event::MouseMoved)
			DefaultContext->injectMousePosition(
				event.mouseMove.x, event.mouseMove.y);
		else if( event.type == sf::Event::MouseButtonPressed)
		{
			DefaultContext->injectMouseButtonDown(
				static_cast<CEGUI::MouseButton>(event.mouseButton.button));
			if(!IsFocused)
			{
				IsFocused = true;
				SetFocus(GraphicsEngine.Window->getSystemHandle());
			}
		}
		else if( event.type == sf::Event::MouseButtonReleased)
		{
			DefaultContext->injectMouseButtonUp(
				static_cast<CEGUI::MouseButton>(event.mouseButton.button));
		}
		else if( event.type == sf::Event::LostFocus)
			IsFocused = false;
		else if( event.type == sf::Event::GainedFocus)
		{
			Utils::ClipCursorWnd(GraphicsEngine.Window);
			IsFocused = true;
		}
		if(IsFocused)
		{
			if( event.type == sf::Event::TextEntered)
			{
				CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(event.text.unicode);
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
							CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(CEGUI::Key::Backspace);
							break;
						}//switch(event.key.code)
					}//if(event.type == sf::Event::KeyPressed)
		}

}

float diff = 0.f; //TODO move to MainLoop()

void GameManager::MainLoop()
{
	float TickTime = 1000.f/TPS, startTime = 0.f, endTime = 0.f;



	while(!IsExitRequired)
	{

		sf::Event event;
		GraphicsEngine.Window->pollEvent(event);
		ProceedDefaultEvent(event);

		if(IsFocused)
		{
			GameState* NewGameState = CurrentGameState->getNewGameState();
			if(NewGameState)
			{
				IsFocused = false;
				delete CurrentGameState;
				CurrentGameState = NewGameState;
				IsFocused = true;
			}
			//GraphicsEngine.Window->pol
			
			static sf::Clock clock;
			startTime = clock.getElapsedTime().asMilliseconds();

			CurrentGameState->logic();
			CurrentGameState->proceedEvent(event);
			Render();

			endTime = clock.getElapsedTime().asMilliseconds();
			diff = endTime-startTime;
			CEGUI::System::getSingleton().injectTimePulse(TickTime);
			if(TickTime-diff > 0)
				Sleep(TickTime-diff);
			
		}	

		else
			Sleep(20);
	}
	delete RenderThread;
}


void GameManager::Render()
{
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

	GraphicsEngine.RenderText("Frame time: "+boost::lexical_cast<std::string>(diff),
		sf::Vector2f(200.f, 0.f), GraphicsEngine.GetFont("arial"), sf::Color(255.f, 0.f,0.f));

	GraphicsEngine.Window->popGLStates();
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


/*void GameManager::MainLoop()
{
	//std::recursive_mutex mutex;
	std::thread* RenderThread = 0, *LogicThread = 0;
	//RenderThread = new std::thread([=](){Render();});
	
	float TickTime = 1000.f/TPS, startTime = 0.f, endTime = 0.f, diff = 0.f;

	GraphicsEngine.Window->setActive(false);
	RenderThread = new std::thread([&](){while(true){Render();}});
	//LogicThread = new std::thread([&](){while(true){}});

	while(true)
	{

		//LostFocus waiting routine
		while(GetFocus() != GraphicsEngine.Window->getSystemHandle())
		{
			sf::IntRect wnd(GraphicsEngine.Window->getPosition().x, GraphicsEngine.Window->getPosition().y,
				GraphicsEngine.Window->getPosition().x+GraphicsEngine.Window->getSize().x, GraphicsEngine.Window->getPosition().y+GraphicsEngine.Window->getSize().y);
			IsFocused = false;
			sf::sleep(sf::milliseconds(50));
			sf::Event event;
			GraphicsEngine.Window->pollEvent(event);
			ProceedDefaultEvent(event);
			if(event.type == sf::Event::MouseButtonPressed)
				//if(wnd.contains(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)))
				{
					SetFocus(GraphicsEngine.Window->getSystemHandle());
					IsFocused = true;
					Utils::ClipCursorWnd(GraphicsEngine.Window);
				}
		}

		if(IsFocused)
		{
			GameState* NewGameState = CurrentGameState->getNewGameState();
			if(NewGameState)
			{
				//delete RenderThread;
				//delete LogicThread;
				delete CurrentGameState;
				CurrentGameState = NewGameState;
				//RenderThread = new std::thread([=](){Render();});
				//LogicThread = new std::thread([=](){CurrentGameState->logic();});
			}
			//GraphicsEngine.Window->pol
			


			//CurrentGameState->logic();
						static sf::Clock clock;
			startTime = clock.getElapsedTime().asMilliseconds();

			CurrentGameState->logic();

			endTime = clock.getElapsedTime().asMilliseconds();
			diff = endTime-startTime;
			//std::cout << diff << endl;
			if(TickTime-diff > 0)
				Sleep(TickTime-diff);
			
			
	/*				static int startTime, endTime, diff;
		static sf::Clock clock;
		startTime = clock.getElapsedTime().asMilliseconds();

		CurrentGameState->logic();

		endTime = clock.getElapsedTime().asMilliseconds();
		diff = endTime-startTime;

		do
		{
			
			endTime = clock.getElapsedTime().asMilliseconds();
			diff = endTime-startTime;
		}
		while((TPS-diff) > 0);
			
			
			
			//Render and logical tick
			//CurrentGameState->logic();
		}	
		
	}
}*/