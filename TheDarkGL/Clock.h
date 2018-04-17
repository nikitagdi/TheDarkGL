//---------------------------------------
// Clock.h
// The Dark engine component
// Simple wrapper for windows high perfomance timer
// 2016 Nikita Zaikin
//---------------------------------------
#pragma once
#include <windows.h>
#include <SFML\System\Clock.hpp>

namespace TDEngine
{
	class Clock
	{
	public:
		Clock();
		enum MODE {STANDART, HIGHRES};
		void setMode(MODE);

		//Returns time elapsed since last call of restart() in milliseconds
		float getElapsedTime();
		void restart();
	private:
		LARGE_INTEGER initialTicks;
		LARGE_INTEGER finalTicks;
		LARGE_INTEGER frequency;
		sf::Clock clock;
		MODE mode;
	};

}