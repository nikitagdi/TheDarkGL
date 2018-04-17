#include "Player.h"
#include "Game.h"
extern TDEngine::GameManager TDEngine::GameEngine;

namespace TDGame
{
	Player::Player() : Player("Player")
	{

	}

	Player::~Player(){}


	void Player::ProceedEvent(TDEngine::Event& event)
	{
		if(event.name == "event_buttonPressed")
			switch (*((sf::Keyboard::Key*)event.Parameters[0].second))
			{
			case sf::Keyboard::Up:
				UpdateMovement(Entity::MOVEUP);
				break;
			case sf::Keyboard::Down:
				UpdateMovement(Entity::MOVEDOWN);
				break;
			case sf::Keyboard::Left:
				UpdateMovement(Entity::MOVELEFT);
				break;
			case sf::Keyboard::Right:
				UpdateMovement(Entity::MOVERIGHT);
				break;
			default:
				break;
			}
		else if(event.name == "event_buttonReleased")
			switch (*((sf::Keyboard::Key*)event.Parameters[0].second))
			{
			case sf::Keyboard::Up:
				UpdateMovement(Entity::MOVEUP, false);
				break;
			case sf::Keyboard::Down:
				UpdateMovement(Entity::MOVEDOWN, false);
				break;
			case sf::Keyboard::Left:
				UpdateMovement(Entity::MOVELEFT, false);
				break;
			case sf::Keyboard::Right:
				UpdateMovement(Entity::MOVERIGHT, false);
				break;
			default:
				break;
			}

		Entity::ProceedEvent(event);
	}

	Player::Player(std::string name, float X, float Y, unsigned int size) : Entity(name, X, Y, size)
	{
		TDEngine::GameEngine.EventEngine.Subscribe(this, "event_buttonPressed");
		TDEngine::GameEngine.EventEngine.Subscribe(this, "event_buttonReleased");

		animations.RegisterAnimation(ANIMATIONS::MOVESOUTH, TDEngine::GameEngine.GraphicsEngine.GetAnimation("PlayerMoveSouth"));
		animations.RegisterAnimation(ANIMATIONS::MOVEWEST, TDEngine::GameEngine.GraphicsEngine.GetAnimation("PlayerMoveWest"));
		animations.RegisterAnimation(ANIMATIONS::MOVEEAST, TDEngine::GameEngine.GraphicsEngine.GetAnimation("PlayerMoveEast"));
		animations.RegisterAnimation(ANIMATIONS::MOVENORTH, TDEngine::GameEngine.GraphicsEngine.GetAnimation("PlayerMoveNorth"));
		animations.Deactivate();
		animations.ChangeState(ANIMATIONS::MOVESOUTH);
		//TDEngine::GameEngine.EventEngine.
	}
}