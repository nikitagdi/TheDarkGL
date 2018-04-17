#include "GameStateGame.h"
#include "Game.h"
#include "GameObject.h"
#include "Mob.h"
#include <Windows.h>
#include <mutex>



extern TDEngine::GameManager TDEngine::GameEngine;

namespace TDGame
{

	static float x = 0.f, y = 0.f;
	std::vector<Entity*> mobs;

	GameStateGame::GameStateGame() 
	{
			SYSTEMTIME Time;
			GetLocalTime(&Time);
			srand(Time.wMilliseconds);
			
	
			map = new Map(100,100);	
			mapRedactor.setMap(map);
			ViewX = ViewY = 0;

			map->loadMap("level4.tdmap");

			//map->clearMap();
			Mob* testMob = new Mob();
			testMob->setMap(map);
			testMob->SetAnimation(Entity::ANIMATIONS::MOVESOUTH, TDEngine::GameEngine.GraphicsEngine.GetAnimation("SkeletonMoveSouth"));
			testMob->SetAnimation(Entity::ANIMATIONS::MOVENORTH, TDEngine::GameEngine.GraphicsEngine.GetAnimation("SkeletonMoveNorth"));
			testMob->SetAnimation(Entity::ANIMATIONS::MOVEEAST, TDEngine::GameEngine.GraphicsEngine.GetAnimation("SkeletonMoveEast"));
			testMob->SetAnimation(Entity::ANIMATIONS::MOVEWEST, TDEngine::GameEngine.GraphicsEngine.GetAnimation("SkeletonMoveWest"));
			testMob->setTexture(*TDEngine::GameEngine.GraphicsEngine.GetTexture("MOB"));
			map->addObject(*testMob);

			//mapRedactor.UpdateMenu();


	}

	void GameStateGame::renderScene()
	{	
		map->renderMap(x, y);

		mapRedactor.Render();
	}

	void GameStateGame::logic() 
	{

		map->update();

		TDEngine::Event ev = TDEngine::GameEngine.EventEngine.GetEventPattern("event_Update");
		float time = 33.f;
		ev.Parameters[0].second = &time;
		TDEngine::GameEngine.EventEngine.SendEvent(ev);
	}


	void GameStateGame::proceedEvent(sf::Event& event)
	{
		sf::RenderWindow* wnd = TDEngine::GameEngine.GraphicsEngine.Window;
		int TileSize = TDEngine::GameEngine.GetSetting<int>("TileSize");
		int width = wnd->getView().getSize().x;
		int height = wnd->getView().getSize().y;
		float aspect = (float)height/(float)width;
		sf::Vector2i mousePos = sf::Mouse::getPosition(*wnd);
		sf::Vector2u windowsize = wnd->getSize();

		
		
		if(event.type == sf::Event::KeyPressed)
		{
			switch(event.key.code)
			{
			case sf::Keyboard::Add:
				Entity::MaxStateProgress++;
				break;
			case sf::Keyboard::Subtract:
				Entity::MaxStateProgress--;
				break;
			case sf::Keyboard::F5:
				map->saveMap("level4.tdmap");
				break;
			case sf::Keyboard::F9:
				map->loadMap("level4.tdmap");
				break;
			case sf::Keyboard::F6:
				map->clearMap();
				break;
			case sf::Keyboard::F7:
				map->optimize();
				break;
			}//switch(event.key.code)
		}//if(event.type == sf::Event::KeyPressed) 

		else if(event.type == sf::Event::MouseWheelMoved)
		{
			if(!mapRedactor.IsRedactorActive())
			{
				//TDEngine::GameEngine.ReportError("ViewX is " + boost::lexical_cast<std::string>(event.mouseWheel.delta*ZoomSpeed*(windowsize.x/windowsize.y)));
				//ViewX -= event.mouseWheel.delta*ZoomSpeed*(windowsize.x/windowsize.y);
//				ViewY -= event.mouseWheel.delta*ZoomSpeed*(windowsize.x/windowsize.y);
			}
		}

		sf::Vector2f center = TDEngine::GameEngine.GraphicsEngine.Window->getView().getCenter();

		if(event.type == sf::Event::MouseButtonPressed)
		{
			Tile* obj = map->getObjectByMouseCoordinates(mousePos.x, mousePos.y);
			if(obj)
				if(!TDEngine::GameEngine.EventEngine.IsObjectSubscribed(obj, "event_changeColor"))
					TDEngine::GameEngine.EventEngine.Subscribe(obj, "event_changeColor");
				else
					TDEngine::GameEngine.EventEngine.Unsubscribe(obj, "event_changeColor");
		}
		
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			sf::Event ev;
			ev.type = sf::Event::MouseButtonPressed;
			ev.mouseButton.x = mousePos.x;
			ev.mouseButton.y = mousePos.y;
			ev.mouseButton.button = sf::Mouse::Left;
			mapRedactor.ProceedEvent(ev);
		}
		if (event.type == sf::Event::MouseButtonReleased)
			if (event.mouseButton.button == sf::Mouse::Button::Right)
				mapRedactor.ProceedEvent(event);
		/*if(GetAsyncKeyState(VK_LBUTTON))
		{
			POINT p;
			event.type = sf::Event::MouseLeft;
			GetCursorPos(&p);
			ScreenToClient(TDEngine::GameEngine.GraphicsEngine.Window->getSystemHandle(), &p);
			event.mouseButton.x = p.x; event.mouseButton.y = p.y;
			mapRedactor.ProceedEvent(event);
		}*/
		
	}
	}