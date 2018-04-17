#include "Mob.h"
#include "GameStateGame.h"


namespace TDGame
{
	Mob::Mob(std::string name, float X, float Y, unsigned int size, float MS, RPSAttributes attr, Map* map, unsigned int sightR)
		: Entity(name, X, Y, size, MS, attr, map), sightRadius(sightR)
	{
		animations.Deactivate();
		animations.ChangeState(ANIMATIONS::MOVESOUTH);
		
	}


	void Mob::ProceedEvent(TDEngine::Event& event)
	{
		Entity::ProceedEvent(event);

		if (event.name.compare("event_Update") == 0)
		{
			Mob::Update(*((float*)event.Parameters[0].second));
		}
	}


	void Mob::Update(float ms)
	{
		if (!world)
		{
			TDEngine::GameEngine.LogEngine.LogError("World pointer is null. Path finding interrupted.", "MobAI");
			return;
		}

		TDEngine::AStar::Vec2i selfCoord   = { (int)(getPosition().x / 32), (int)(getPosition().y / 32) };
		

		if (!targetPath.empty() && target)
		{
			TDEngine::AStar::Vec2i targetCoord = { (int)(target->getPosition().x / 32), (int)(target->getPosition().y / 32) };
			//Rebuilding path if target changed its position
			if (!(targetPath.front() == targetCoord) )
				targetPath = world->getPath(selfCoord, targetCoord);
			
			TDEngine::AStar::Vec2i coord = targetPath.back();
//			if (coord.x - selfCoord.x == 0 && coord.y - selfCoord.y == 0 && targetPath.size() > 1)
//			{
//				coord = targetPath.back();
//				targetPath.pop_back();
//			}
			UpdateMovement(sf::Vector2f(coord.x - selfCoord.x, coord.y - selfCoord.y));
			if(selfCoord.x == coord.x && selfCoord.y == coord.y)
				targetPath.pop_back();
			
		}
		else
		{
			//finding new target
			for (auto entity : world->getEntities())
				if(entity && entity != this)
				{
					targetPath = world->getPath({ (int)(getPosition().x / 32), (int)(getPosition().y / 32) }, { (int)(entity->getPosition().x / 32), (int)(entity->getPosition().y / 32) });
					if (targetPath.size() - 1 <= sightRadius)
					{
						target = entity;
						break;
					}
				}
			if (targetPath.size() - 1 > sightRadius)
			{
				targetPath.clear();
				target = nullptr;
			}

		}
		//Movement update
		Entity::Update(ms);
	}
}