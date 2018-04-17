#pragma once
#include "EventManager.h"
#include "Tile.h"
#include <string>
#include <cstdlib>
#include "Game.h"
#include "Utils.h"

extern TDEngine::GameManager TDEngine::GameEngine;

namespace TDGame
{
	//Warning!
	//The class is redundant and is not being used in game
	//TODO: Delete
	class GameObject : public TDEngine::EventReceiver
	{
	public:
		GameObject(float X, float Y, std::string _type, std::string _name = "EMPTY OBJECT", sf::Texture *sprite = 0) : x(X), y(Y), name(_name), Texture(sprite)
		{
			//x = 0.0f, y = 0.0f;
		//	tile = Tile(0x2588, sf::Color(255, 0, 0), 0);
			//Model.setFont(TDEngine::GameManager::GraphicsEngine.GetFont("arial"));
			//Model.setString(tile.symbol);
			//Model.setColor(tile.color);
			//Model.setCharacterSize(TDEngine::GameEngine.GetSetting<int>("TileSize"));
			//Model.setRotation(tile.angle);
			//Texture = sf::Texture(sprite);
			Model.setTexture(Texture, true);
			Model.setPosition(sf::Vector2f(x,y));
			Model.setSize(sf::Vector2f(32,32));
		//	Model.rotate(120.5f);
		//	Model.scale(1.5f, 1.5f);
			//Model.setColor(sf::Color::Green);
		}
		//virtual ~GameObject(void);
		virtual void destroy() {delete this;}
		virtual void tick()    
		{
		}

		virtual void ProceedEvent(TDEngine::Event& event)
		{
			if(event.name == "event_changeColor")
			{
				//auto func = static_cast<int (*)()>(event.Parameters[0].second);
				Model.setFillColor(sf::Color(*(int*)event.Parameters[0].second, *(int*)event.Parameters[0].second, *(int*)event.Parameters[0].second));
			}
			else if(event.name == "event_changeTile")
			{
				if(event.Parameters[0].second != 0)
				{
				//std::wstring sym = *((std::wstring*)event.Parameters[0].second);
				//sf::Texture* tex = TDEngine::GameEngine.GraphicsEngine.GetTexture(TDEngine::Utils::WstringToString(sym));
				sf::Texture* tex = (sf::Texture*)event.Parameters[0].second;
				//x = (x/Model.getTexture()->getSize().x)*tex->getSize().x;
				//y = (y/Model.getTexture()->getSize().y)*tex->getSize().y;
				
				Model.setTexture(tex, true);
				Model.scale(2.f, 2.f);
				Model.setPosition(sf::Vector2f((x-y),(y+x)/2));
				}
				//Model.setTextureRect(tex->getSize());
		}	
		}

		virtual void render()
		{
			//sf::RenderStates renderStates(sf::Transform().translate(sf::Vector2f(x*TDEngine::GameEngine.GetSetting<int>("TileSize"),y*TDEngine::GameEngine.GetSetting<int>("TileSize"))).rotate(tile.angle));
			TDEngine::GameManager::GraphicsEngine.Window->draw(Model);
		}

		sf::FloatRect GetGlobalRect()
		{
			return Model.getGlobalBounds();
		}

		float x, y;
		sf::Texture *Texture;
		sf::RectangleShape Model;
		// name is ingame name of object. Type is name for all object's recources
		std::string name, type;
	};
}

