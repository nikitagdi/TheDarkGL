#pragma once
#include "gamestate.h"

namespace TDGame
{
	class GameStateInit :
		public TDEngine::GameState
	{
	public:
		GameStateInit(void);
		~GameStateInit(void);
		void logic();
		void renderScene();
		void proceedEvent() {}
	};
}

