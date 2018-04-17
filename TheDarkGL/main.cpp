#include "Game.h"
#include <SFML\Network.hpp>

TDEngine::GameManager TDEngine::GameEngine;

int main(int* argc, char** argv)
{
	TDEngine::GameEngine.Init();
	TDEngine::GameEngine.MainLoop();
}