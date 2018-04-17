#pragma once
#include "GameState.h"
#include "Map.h"
#include "MapRedactor.h"
#include "player.h"


namespace TDGame
{
	class GameStateGame 
		: public TDEngine::GameState
	{
	public: 
		GameStateGame()    ;
		~GameStateGame()   override {}
		void logic()       override;
		void renderScene() override;
		void proceedEvent(sf::Event&) override;

		Map* map;
	private:
		float ViewX, ViewY;
		MapRedactor mapRedactor;
	};

}