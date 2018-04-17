#pragma once
#include "Map.h"
#include <string>


namespace TDGame
{
	class MapManager
	{
	public:
		void LoadMaps(std::string);

		Map* GetMap(int id)
		{
			if(id <= (Maps.size()-1) && id >= 1) 
				return Maps[id];
			return 0;
		}


		Map* GetMap(std::string name)
		{
			for(auto map : Maps)
				if(map)
					if(map->name == name)
						return map;
			return 0;
		}


		int GetMapID(Map* map)
		{
			for(int i = 0; i < Maps.size(); i++)
					if(Maps[i] == map)
						return i;
			return -1;
		}
	private:
		std::vector<Map*> Maps;
	};
}