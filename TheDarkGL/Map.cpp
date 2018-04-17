#include "Map.h"
#include "INI1.31.h"
#include <exception>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>


extern TDEngine::GameManager TDEngine::GameEngine;


namespace TDGame
{
	

	Map* Map::MapInstance = 0;
	std::vector<TerrainTile*> Map::TerrainBlocks;

	

	Map::Map(int x, int y) : sizeX(x), sizeY(y)
	{
		static bool init = false;
		MapInstance = this;

		
		for(int i = 0; i < TerrainListSize; i++)
			usedIDs[i] = false;


		if(!init)
		{
			TerrainTile defaultTerrain("grass", true, 0.f, 0.f, 0);
			defaultTerrain.setTexture(*TDEngine::GameEngine.GraphicsEngine.GetTexture("grass_top"));
			TerrainBlocks.resize(256);
	/*		for(int i = 0; i < (int)TerrainBlocks.size(); i++)
			{
				defaultTerrain.setID(i);
				TerrainBlocks[i] = new TerrainTile(defaultTerrain);
			}*/
			defaultTerrain = TerrainTile("DEFAULT", true, 0.f, 0.f, 1);
			defaultTerrain.setTexture(*TDEngine::GameEngine.GraphicsEngine.GetTexture("tileset_32x32_terrain_10"));
			TerrainBlocks[0] = new TerrainTile(defaultTerrain);
			init = true;
		}

		passabilityMap.setWorldSize({ x, y });
		passabilityMap.setDiagonalMovement(true);
	}


	Map::~Map(void)
	{
	}


	/*int Map::getObjectID(GameObject* obj)
	{
		if(obj)
			for(int i = 0; i < (int)objects.size(); i++)
				if(objects[i] == obj)
					return i;
		return -1;
	}*/

	Tile* Map::getObjectByMouseCoordinates(float x, float y)
	{
		sf::Vector2u windowsize = TDEngine::GameEngine.GraphicsEngine.Window->getSize();
		sf::Vector2<float> mouseClickPos = cam.windowCoordToCamera(sf::Vector2f(x, y), sf::Vector2f(windowsize.x, windowsize.y));

		Sector* sector = getSector(mouseClickPos.x / Sector::size, mouseClickPos.y / Sector::size);

		if (!sector)
			return 0;

		for (auto tile : sector->getTerrain())
			if (tile)
				if (tile->getGlobalBounds().contains(mouseClickPos))
					return tile;

		for (auto obj : Entities)
			if (obj)
			{
				sf::Rect<float> ObjCBox; ObjCBox.left = obj->getGlobalBounds().left;
				ObjCBox.top = obj->getGlobalBounds().top; ObjCBox.width = obj->getGlobalBounds().width;
				ObjCBox.height = obj->getGlobalBounds().height;
				if (ObjCBox.contains(mouseClickPos))
					return obj;
			}

		return 0;
	}

	std::vector<Tile*> Map::getObjectsByMouseCoordinates(float x, float y)
	{
		sf::Vector2u windowsize = TDEngine::GameEngine.GraphicsEngine.Window->getSize();
		sf::Vector2<float> mouseClickPos = cam.windowCoordToCamera(sf::Vector2f(x, y), sf::Vector2f(windowsize.x, windowsize.y));
		std::vector<Tile*> toRet;

		Sector* sector = getSector(mouseClickPos.x / Sector::size, mouseClickPos.y / Sector::size);

		for (auto tile : sector->getTerrain())
			if (tile)
				if (tile->getGlobalBounds().contains(mouseClickPos))
					toRet.push_back(tile);

		for (auto obj : Entities)
			if (obj)
			{
				sf::Rect<float> ObjCBox; ObjCBox.left = obj->getGlobalBounds().left;
				ObjCBox.top = obj->getGlobalBounds().top; ObjCBox.width = obj->getGlobalBounds().width;
				ObjCBox.height = obj->getGlobalBounds().height;
				if (ObjCBox.contains(mouseClickPos))
					toRet.push_back(obj);
			}

		return toRet;
	}

	/*GameObject* Map::getObject(int ID)
	{
		if(ID <= (objects.size()-1) && ID >= 1) 
			return objects[ID];
		return 0;
	}*/

	Tile* Map::getObject(std::string name)
	{
		for(int i = 0; i < (int)Terrain.size(); i++)
			if(Terrain[i])
				if(Terrain[i]->GetName() == name)
					return Terrain[i];
		for(int i = 0; i < (int)Entities.size(); i++)
			if(Entities[i])
				if(Entities[i]->GetName() == name)
					return Entities[i];
		return 0;
	}

	Tile* Map::getObject(float x, float y)
	{
		for(auto sectors : Sectors)
			for(auto sector : sectors)
				if(sector)
					for(auto terrain : sector->getTerrain())
						if(terrain->getPosition().x == x && terrain->getPosition().y == y)
							return terrain;
		return 0;
	}

	std::vector<Tile*> Map::getObjects(std::string name)
	{
		std::vector<Tile*> ret;

		for(int i = 0; i < (int)Terrain.size(); i++)
			if(Terrain[i])
				if(Terrain[i]->GetName() == name)
					ret.push_back(Terrain[i]);
		for(int i = 0; i < (int)Entities.size(); i++)
			if(Entities[i])
				if(Entities[i]->GetName() == name)
					ret.push_back(Entities[i]);					
		return ret;
	}

	std::vector<Tile*> Map::getObjects(float x, float y)
	{
		std::vector<Tile*> ret;

		for(int i = 0; i < (int)Terrain.size(); i++)
			if(Terrain[i])
				if(Terrain[i]->getGlobalBounds().contains(sf::Vector2f(x,y)))
					ret.push_back(Terrain[i]);
		for(int i = 0; i < (int)Entities.size(); i++)
			if(Entities[i])
				if(Entities[i]->getPosition().x == x && Entities[i]->getPosition().y == y)
					ret.push_back(Entities[i]);	
		
		return ret;
	}


	Sector* Map::getSector(int x, int y)
	{
		if( x < 0 || y < 0)
			return nullptr;
		if(x+1 > Sectors.size())
			Sectors.resize(x+1);
		if(y+1 > Sectors[x].size())
			Sectors[x].resize(y+1);
		
		return Sectors[x][y];
	}


	void Map::addObject(TerrainTile& obj)
	{
		Sector* sector = getSector(obj.getPosition().x/Sector::size, obj.getPosition().y/Sector::size);
		if(!sector)
		{
			sector = new Sector(obj.getPosition().x/Sector::size, obj.getPosition().y/Sector::size);
			try
			{
				Sectors.at(sector->x).at(sector->y) = sector;
			}
			catch (std::exception& ex)
			{
				std::string message = ex.what();
				TDEngine::GameEngine.LogEngine.LogError("Exception caught during adding object "+obj.GetName(), "MapManager");
				TDEngine::GameEngine.LogEngine.LogError("Exception message: "+message, "MapManager");
			}
		}
		sector->Terrain.push_back(&obj);
	}

	void Map::addObject(Entity& obj)
	{
		for( auto object : Entities)
			if(object == 0)
			{
				object = &obj;
				return;
			}
		Entities.push_back(&obj);
	}

	void Map::update()
	{
		for (auto sectors : Sectors)
			for (auto sector : sectors)
				if (sector)
					for (auto tile : sector->getTerrain())
						if (tile)
							if (!tile->IsPassable())
								passabilityMap.addCollision({ (int)(tile->getPosition().x / 32),  (int)(tile->getPosition().y / 32) });

		cam.Update();

	//	for(auto entity : Entities)
	//		entity->Update();
	}



	void Map::clearMap()
	{
		for (auto entity : Entities)
			delete entity;
		Entities.clear();

		for (auto sectors : Sectors)
			for (auto sector : sectors)
				if (sector)
				{
					sector->clear();
					delete sector;
				}
		Sectors.clear();
	}


	void Map::loadMap(std::string FileName)
	{
		TDEngine::GameEngine.LogEngine.LogInfo("Loading map "+FileName, "Map");

		clearMap();

		std::ifstream mapStream("Data//Maps//" + FileName, std::ifstream::in | std::ifstream::binary);
		boost::archive::binary_iarchive ia(mapStream);
		ia & *this;
		mapStream.close();
	}


	void Map::saveMap(std::string FileName)
	{
		std::ofstream mapStream("Data//Maps//" + FileName,std::ofstream::out | std::ofstream::binary);
		{
			boost::archive::binary_oarchive oa(mapStream);
			oa & *this;
		}
		mapStream.close();
	}



	void Map::setSize(int x, int y)
	{
		sizeX = x;
		sizeY = y;
	}


	sf::Vector2i Map::getSize() const
	{
		return sf::Vector2i(sizeX, sizeY);
	}


	TerrainTile& Map::getTilePattern(int id)
	{
		TerrainTile* ret = nullptr;
		try
		{
			ret = TerrainBlocks.at(id);
			if(ret)
				return *ret;
		}
		catch(std::exception& ex)
		{
			TDEngine::GameEngine.LogEngine.LogError(std::string(ex.what())+": Failed to get Terrain block pattern(id "
				+boost::lexical_cast<std::string>(id)+") ", "Map");
		}
		ret = TerrainBlocks[0];
		return *ret;
	}


	void Map::setTilePattern(TerrainTile* terrainTile)
	{
		if(!terrainTile)
			return;
		if(TerrainBlocks[terrainTile->getID()])
		{
			TDEngine::GameEngine.LogEngine.LogError("Block already exists(id "
				+boost::lexical_cast<std::string>(terrainTile->getID())+") ", "Map");
			return;
		}
		try
		{
			TerrainBlocks.at(terrainTile->getID()) = new TerrainTile(*terrainTile);
		}
		catch(std::exception& ex)
		{
			TDEngine::GameEngine.LogEngine.LogError(std::string(ex.what())+": Invalid ID(id "
				+boost::lexical_cast<std::string>(terrainTile->getID())+") ", "Map");
		}
	}


	void Map::renderMap(float x, float y)
	{
		TDEngine::GameEngine.GraphicsEngine.Window->setView(sf::View(sf::Vector2f(x, y),
			sf::Vector2f(TDEngine::GameEngine.GraphicsEngine.getWidth() + TDEngine::GameEngine.GraphicsEngine.ViewX, TDEngine::GameEngine.GraphicsEngine.getHeight() + TDEngine::GameEngine.GraphicsEngine.ViewY)));
		TDEngine::GameEngine.GraphicsEngine.Window->setView(cam.getView());
		
		TDEngine::GameEngine.GraphicsEngine.Window->pushGLStates();
		float _x = TDEngine::GameEngine.GraphicsEngine.Window->getView().getCenter().x;
		float _y = TDEngine::GameEngine.GraphicsEngine.Window->getView().getCenter().y;
		int width = (TDEngine::GameEngine.GraphicsEngine.Window->getView().getSize().x / 2);
		int height = (TDEngine::GameEngine.GraphicsEngine.Window->getView().getSize().y / 2);
		auto viewport2 = sf::FloatRect(_x - width, _y - height, width + _x, height + _y);
		auto viewport = cam.getViewport();
		int count = 0;

		for (int i = viewport.left / static_cast<float>(Sector::size); i < ceil((viewport.left+viewport.width) / static_cast<float>(Sector::size)); i++)
			for (int j = viewport.top / static_cast<float>(Sector::size); j < ceil((viewport.top + viewport.height) / static_cast<float>(Sector::size)); j++)
			{
				if (i < Sectors.size())
					if (j < Sectors[i].size())
					{
						auto sector = Sectors.at(i).at(j);
						if (sector)
							for (auto obj : sector->getTerrain())
								if (obj)
									//Viewport intersection is ineffective
									//if (viewport.intersects(obj->getGlobalBounds()))
								{
									TDEngine::GameManager::GraphicsEngine.Window->draw(*obj);
								//	count++;
								}
					}
			}

		//TDEngine::GameEngine.LogEngine.LogInfo("Totally rendered: " + boost::lexical_cast<std::string>(count) + "tiles", "MapRenderer");

		for (auto obj : getEntities())
			if (viewport.intersects(obj->getGlobalBounds()))
			{
				//sf::Vector2f displacement = obj->GetUpdatedDisplacement(TDEngine::GameEngine.getActualTime())/2.f;
				TDEngine::GameManager::GraphicsEngine.Window->draw(*obj);
			}
		TDEngine::GameEngine.GraphicsEngine.Window->popGLStates();

	}



	void Map::optimize()
	{
	/*	for (auto sectors : Sectors)
			for (auto sector : sectors)
				if (sector)
					for (auto tile : sector->getTerrain())
						for (auto _sectors : Sectors)
							for (auto _sector : _sectors)
								if (_sector)
									for (auto _tile : _sector->getTerrain())
										if(_tile && tile)
										if ((*tile == *_tile) && (tile != _tile))
										{
											removeObject(_tile);
											delete _tile;
										}*/
		std::map<std::pair<float, float>, std::vector<TerrainTile*>> tiles;

		for (auto sectors : Sectors)
			for (auto sector : sectors)
				if (sector)
					for (auto tile : sector->getTerrain())
						if (tile)
							tiles[std::pair<float, float>(tile->getPosition().x, tile->getPosition().y)].push_back(tile);

		TerrainTile* previous = 0;
		for (auto _tiles : tiles)
			for (auto tile : _tiles.second)
				if (previous)
					if (*tile == *previous)
					{
						removeObject(tile);
						delete tile;
					}
					else
						previous = tile;
				else
					previous = tile;
	}


	void Map::removeObject(TerrainTile* obj)
	{
		for(auto sectors : Sectors)
			for(auto sector : sectors)
				if(sector)
					{
						std::vector<TerrainTile*>& container = sector->getTerrain();
						std::vector<TerrainTile*>::iterator position = std::find(container.begin(), container.end(), obj);
						if (position != container.end()) // == myVector.end() means the element was not found
							container.erase(position);
					}
	}

	bool Map::checkPassability(float x, float y)
	{
		Sector* sector = getSector((int)(x/Sector::size), (int)(y / Sector::size));

		if (!sector)
			return true;

		for (auto tile : sector->getTerrain())
			if (tile)
				if (tile->getGlobalBounds().contains(sf::Vector2f(x, y)) && !tile->IsPassable())
					return false;

		return true;
	}
}
