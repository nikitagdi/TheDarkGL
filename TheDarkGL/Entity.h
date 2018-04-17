#pragma once
#include "RPSAttributes.h"
#include "Tile.h"

namespace TDGame
{
	class Map;
	class Entity : public Tile
	{
		friend class boost::serialization::access;
	public:
		//Entity(std::string name, float MaxHP, float MaxMP, float BaseAttackSpeed,
		//	float BaseMoveSpeed, float BaseArmor, RPGAttributes Attributes);
		Entity(std::string = "NONAME ENTITY", float X = 0.f, float Y = 0.f, unsigned int size =  32, float MS = 1000.f
			, RPSAttributes attr = RPSAttributes(100.f, 20.f), Map* map = nullptr);
		virtual ~Entity() {}

		enum ACTION {NOTHING, MOVEUP, MOVEDOWN, MOVELEFT, MOVERIGHT};
		//enum for animations
		enum ANIMATIONS {MOVESOUTH = 100, MOVEWEST, MOVEEAST, MOVENORTH};
		static int MaxStateProgress;

		virtual void DoAction(ACTION);


		//Updates kinematic properties of entity
		//true - accelerate, false - decelerate(stops movement in the given direction)
		void UpdateMovement(ACTION, bool = true);
		//Manually set movement direction vector
		void UpdateMovement(sf::Vector2f);
		//Returns predicted object displacement using time elapsed from last update
		sf::Vector2f GetUpdatedDisplacement(float);
		bool IsMoving() const { return moveDirection.x || moveDirection.y; }

		const RPSAttributes& GetAttributes() const {return Attributes;}


		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & boost::serialization::base_object<TDGame::Tile>(*this);
			try {
				ar & Attributes;
			}
			catch (boost::archive::archive_exception& e) { TDEngine::GameEngine.LogEngine.LogError("Error serializing"); }
		}

		virtual void ProceedEvent(TDEngine::Event&);
	protected:
		//Update all properties of Entity (movement, etc.)
		virtual void Update(float ms);
	private:
		//Role play stuff
		RPSAttributes Attributes;

		//Kinematics
		float MoveSpeed;
		sf::Vector2f moveDirection;

		ACTION State;
		int    StateProgress;
		
	};
}

