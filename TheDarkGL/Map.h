#pragma once
#include "Terrain.h"
#include "Entity.h"
#include "Player.h"
#include <vector>
#include <string>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include "Camera.h"
#include "AStar.hpp"

namespace TDGame
{
	class Sector
	{
		friend class Map;
		friend class boost::serialization::access;
	public:

		static const int size = 64;
		Sector(int _x, int _y) : x(_x), y(_y) {}
		Sector() {}
		std::vector<TerrainTile*>& getTerrain()
		{
			return Terrain;
		}

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & x;
			ar & y;
			ar & Terrain;
			
		}

		int getX() { return x; }
		int getY() { return y; }
		sf::FloatRect getRect() { return sf::FloatRect(x*size, y*size, size, size); }
		void clear() {
			if (!Terrain.empty())
			{
				for (auto terrainTile : Terrain)
					delete terrainTile;
				Terrain.clear();
			}
		}

	private:
		std::vector<TDGame::TerrainTile*> Terrain;
		int x, y;
	};


	class Map
	{
		friend class boost::serialization::access;
	public:
		
		Map(int x, int y);
		Map() {}
		~Map(void);

		bool loadMapFromFile(std::string);

	//	int getObjectID(Tile*);
		//Get object by ID
	//	Tile* getObject(int);
		Tile* getObjectByMouseCoordinates(float, float);
		//returns vector of all objects containing given coordinate
		std::vector<Tile*> getObjectsByMouseCoordinates(float, float);
		//Get single object by name
		Tile* getObject(std::string);
		//Get single object by coordinates
		Tile* getObject(float, float);
		//Get all objects with this name
		std::vector<Tile*> getObjects(std::string);
		//Get all objects with this coordinates
		std::vector<Tile*> getObjects(float, float);
		//Get all objects on map
		std::vector<TerrainTile*>& getTerrain() 
		{return Terrain;}
		std::vector<Entity*>& getEntities()
		{return Entities;}
		std::vector<std::vector<Sector*>>& getSectors()
		{return Sectors;}
		//Checks passability of point on map
		bool checkPassability(float, float);
		const TDEngine::Camera& getCamera() const { return cam; }
		TDEngine::AStar::CoordinateList getPath(TDEngine::AStar::Vec2i source_, TDEngine::AStar::Vec2i target_)
		{
			TDEngine::GameEngine.LogEngine.LogInfo("Finding path to: "+boost::lexical_cast<std::string>(target_.x)+", "+ boost::lexical_cast<std::string>(target_.y));
			return passabilityMap.findPath(source_, target_);
		}


		template<class Archive>
		void save(Archive & ar, const unsigned int version) const
		{
			ar.template register_type<Player>();
			ar << name;
			ar << sizeX;
			ar << sizeY;
			ar << Sectors;
			ar << Entities;
		}

		template<class Archive>
		void load(Archive & ar, const unsigned int version)
		{
			ar.template register_type<Player>();
			ar >> name;
			ar >> sizeX;
			ar >> sizeY;
			ar >> Sectors;
			ar >> Entities;
		}
		BOOST_SERIALIZATION_SPLIT_MEMBER()

		Sector* getSector(int x, int y);
		void setSize(int, int);
		sf::Vector2i getSize() const;

		void addObject(TerrainTile&);
		void addObject(Entity&);
		void removeObject(TerrainTile*);
		void update();
		void renderMap(float x, float y);

		void loadMap(std::string FileName);
		void saveMap(std::string FileName);
		void clearMap();
		//Optimizes map(deletes same tiles)
		void optimize();

		static TerrainTile& getTilePattern(int id);
		static void setTilePattern(TerrainTile* terrainTile);

		std::string name;
		static const int TerrainListSize = 2048;
		bool usedIDs[TerrainListSize];

		static Map* MapInstance;

	private:
		static std::vector<TerrainTile*> TerrainBlocks; // blocks for terrain patterns(used for saving and loading maps)
														// IDs from 0 to 255 are reserved
		//Warning! Sector positions in array are their coordinates
		std::vector<std::vector<Sector*>> Sectors;

		std::vector<TerrainTile*> Terrain;
		std::vector<Entity*>      Entities;
		int sizeX, sizeY;
		TDEngine::Camera cam;
		TDEngine::AStar::Generator passabilityMap;
	};
}

