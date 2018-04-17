#define _USE_MATH_DEFINES
#include "Entity.h"
#include "GameStateGame.h"
#include <cmath>


namespace TDGame
{
	

	//
	// Entity
	//

	/*Entity::Entity(std::string name, int MaxHP, int MaxMP, int BaseAttackSpeed,
			int BaseMoveSpeed, int BaseArmor, RPGAttributes Attributes) : Tile(name)
	{
		this->MaxHP = MaxHP;
		this->MaxMP = MaxMP;
		this->BaseAttackSpeed = BaseAttackSpeed;
		this->BaseMoveSpeed = BaseMoveSpeed;
		this->BaseArmor = BaseArmor;
		this->Attributes = Attributes;
	}*/

	int Entity::MaxStateProgress = 3;

	Entity::Entity(std::string name, float X, float Y, unsigned int size, float MS
		, RPSAttributes attr, Map* map) : Tile(name, X, Y, size, map), MoveSpeed(MS)
	{
		Attributes = attr;
		moveDirection = sf::Vector2f(0.f, 0.f);
	}



	bool isPassable(sf::FloatRect rect)
	{
		
		if(dynamic_cast<GameStateGame*>(TDEngine::GameEngine.GetCurrentGameState()))
		{
		
			rect.width /= 1.2;
			rect.height /= 1.2;
			Map* map = ((GameStateGame*)TDEngine::GameEngine.GetCurrentGameState())->map;
			int dx[] = {0, 0, 0, 1, -1, -1, 1, 1, -1};
			int dy[] = {0, -1, 1, 0, 0, -1, -1, 1, 1};
			sf::Vector2f positions[9];
			Sector* sector = map->getSector((rect.left/Sector::size), (rect.top/Sector::size));
			//if(!sector)
			//	return true;

			/*std::vector<TerrainTile*>& objs = ((GameStateGame*)TDEngine::GameEngine.GetCurrentGameState())->map->getTerrain();
			for(auto obj : objs)
				if(obj->getGlobalBounds().intersects(rect) && !obj->IsPassable())
					return false;*/
			std::vector<TerrainTile*> objs;
			std::vector<TerrainTile*> &mapObjs = map->getTerrain();

		//	if((rect.left < sector->getX()*Sector::size) || (rect.left+rect.width > (sector->getX()*Sector::size)+Sector::size)
		//		|| (rect.top < sector->getY()*Sector::size) || (rect.top+rect.height > (sector->getY()*Sector::size)+Sector::size))
				for(int i = 0; i < 9; i++)
				{
					sector = map->getSector((rect.left/Sector::size)+dx[i], (rect.top/Sector::size)+dy[i]);
					if(sector)
						for(auto obj : sector->getTerrain())
							if(!obj->IsPassable() && obj->getGlobalBounds().intersects(rect))
								return false;
				}
		//	else
		//		for(auto obj : sector->getTerrain())
		//			if(!obj->IsPassable() && obj->getGlobalBounds().intersects(rect))
		//				return false;



		/*	objs.push_back((TerrainTile*)map->getObject(((int)rect.left/32)*32, ((int)rect.top/32)*32));
			for(int i = 0; i < 8; i ++)
				objs.push_back((TerrainTile*)map->getObject((((int)rect.left/32)+dx[i])*32, ((((int)rect.top/32)+dy[i])*32)));
			for(auto obj : objs)
				if(obj != 0)
					if(obj->getGlobalBounds().intersects(rect) && !obj->IsPassable())
						return false;*/
		}
		return true;
		/*static std::vector<Tile*> objs;
		if(dynamic_cast<GameStateGame*>(TDEngine::GameEngine.GetCurrentGameState()))
		{
			objs = ((GameStateGame*)TDEngine::GameEngine.GetCurrentGameState())->map->
				getObjects(ceil(x), ceil(y));
			for(auto obj : objs)
				if(dynamic_cast<TerrainTile*>(obj))
					return ((TerrainTile*)obj)->IsPassable();
		}*/
	}


	void Entity::DoAction(ACTION act)
	{
		switch(act)
		{
		default:
			break;
		}
	}


	void Entity::UpdateMovement(ACTION act, bool accel)
	{
		//if (accel)
		//	animations.Activate();
		//else
		//	animations.Deactivate();

		switch (act)
		{
		case MOVEUP:
			 accel ? moveDirection.y -= 1.f : moveDirection.y = 0.f;
			// animations.ChangeState(MOVENORTH);
			break;
		case MOVEDOWN:
			accel ? moveDirection.y += 1.f : moveDirection.y = 0.f;
			//animations.ChangeState(MOVESOUTH);
			break;
		case MOVELEFT:
			accel ? moveDirection.x -= 1.f : moveDirection.x = 0.f;
			////animations.ChangeState(MOVEWEST);
			break;
		case MOVERIGHT:
			accel ? moveDirection.x += 1.f : moveDirection.x = 0.f;
			//animations.ChangeState(MOVEEAST);
			break;
		default:
			break;
		}
	}

	void Entity::UpdateMovement(sf::Vector2f dir)
	{
		moveDirection = dir;
	}


	sf::Vector2f Entity::GetUpdatedDisplacement(float ms)
	{
		if (moveDirection.x != 0.f || moveDirection.y != 0.f)
		{
			float directionLenght = sqrt(moveDirection.x*moveDirection.x + moveDirection.y*moveDirection.y);
			float displacement = MoveSpeed/2 * ms / 1000.f;
			float displacementX = moveDirection.x * displacement / directionLenght
				, displacementY = moveDirection.y * displacement / directionLenght;
			return sf::Vector2f(displacementX, displacementY);
		}
		else 
			return sf::Vector2f(0.f, 0.f);
	}


	void Entity::Update(float ms)
	{
		if (moveDirection.x != 0.f || moveDirection.y != 0.f)
		{
			//Movement
			if (moveDirection.x > 1.f)
				moveDirection.x = 1.f;
			if (moveDirection.x < -1.f)
				moveDirection.x = -1.f;
			if (moveDirection.y > 1.f)
				moveDirection.y = 1.f;
			if (moveDirection.y < -1.f)
				moveDirection.y = -1.f;

			auto displacement = GetUpdatedDisplacement(ms);

			
			sf::FloatRect rect = getGlobalBounds();
			rect.left += displacement.x;
			rect.top += displacement.y;

			//Checking designation tile passability
			//if (isPassable(rect))
			//if(((GameStateGame*)TDEngine::GameEngine.GetCurrentGameState())->map->checkPassability(getPosition().x + displacement.x, getPosition().y + displacement.y))
			if(isPassable(rect))
				setPosition(getPosition().x + displacement.x, getPosition().y + displacement.y);

			//Animation update
			float angle = std::atan2(moveDirection.y, moveDirection.x)*180.f/M_PI + 180.f;
			animations.Activate();
			if(angle >= 315.f || angle < 45.f)
				animations.ChangeState(MOVEWEST);
			else if (angle >= 135.f && angle <= 225.f)
				animations.ChangeState(MOVEEAST);
			else if (angle >= 45.f && angle < 135.f)
				animations.ChangeState(MOVENORTH);
			else if (angle > 225.f && angle < 315.f)
				animations.ChangeState(MOVESOUTH);
			else
				animations.Deactivate();

			updateGlobalBounds();
		}
		else
			animations.Deactivate();
	}

	
	void Entity::ProceedEvent(TDEngine::Event& event)
	{
		Tile::ProceedEvent(event);

		if (event.name.compare("event_Update") == 0)
		{
			Entity::Update(*((float*)event.Parameters[0].second));
		}
	}

}