#include "Terrain.h"

namespace TDGame
{
	TerrainTile::TerrainTile(std::string Name, bool Passable, float X, float Y, int ID) : Tile(Name, X, Y, 32U), id(ID)
	{
		this->Passable = Passable;
	}


	bool TerrainTile::IsPassable()
	{
		return Passable;
	}


	void TerrainTile::ProceedEvent(TDEngine::Event& event) 
	{
		if(event.name == "event_changeColor")
		{
			setColor(sf::Color(*(int*)event.Parameters[0].second, *(int*)event.Parameters[0].second, *(int*)event.Parameters[0].second));
		}
		else if(event.name == "event_changeTile")
		{
			if(event.Parameters[0].second != 0)
			{
			int x = getPosition().x, y = getPosition().y;
			sf::Texture* tex = (sf::Texture*)event.Parameters[0].second;
			setTexture(*tex, true);
		//	setPosition(sf::Vector2f((x-y),(y+x)/2));   for isometric graphics
			setPosition(sf::Vector2f(x,y));
			}
		}	
	}
	
}