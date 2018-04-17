#pragma once
#include "Entity.h"
#include "AStar.hpp"

namespace TDGame
{
	class Mob : public Entity
	{
		friend class boost::serialization::access;

	public:
		Mob(std::string name = "NONAME ENTITY", float X = 0.f, float Y = 0.f, unsigned int size = 32, float MS = 1000.f
			, RPSAttributes attr = RPSAttributes(100.f, 20.f), Map* map = nullptr, unsigned int sightR = 10);
		virtual void ProceedEvent(TDEngine::Event & event);
		virtual ~Mob() {}

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & boost::serialization::base_object<TDGame::Entity>(*this);
		}

	protected:
		//Used to update AI, for movement update Entity::Update is used
		virtual void Update(float ms);
	private:

		//AI Pathfinding
		//Saved path to target
		//First coordinate in vector is position of target
		//Path is rebuilt if target changes its position
		TDEngine::AStar::CoordinateList targetPath;
		//Pointer to target object
		Tile* target;
		//Radius of detection
		unsigned int sightRadius;
	};
}