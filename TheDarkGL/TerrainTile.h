#pragma once
#include "GameObject.h"

namespace TDGame
{
	class TerrainTile : public GameObject
	{
	public:
		TerrainTile(float X, float Y, std::string _type, std::string _name = "EMPTY OBJECT") : GameObject(X, Y, _type, _name) {}

	};
}

