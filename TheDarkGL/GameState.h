#pragma once
#include <SFML\Window.hpp>

namespace TDEngine
{
	class GameState
	{
	public:
		GameState(void) {};
		virtual ~GameState(void) = 0 {};
		virtual void logic() = 0 {};
		virtual void renderScene() = 0 {};
		virtual void proceedEvent(sf::Event&) = 0 {}
		GameState* getNewGameState() {return NewGameState;}

	protected:
		GameState* NewGameState;

	};
}

