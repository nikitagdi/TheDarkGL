#include "Clock.h"

namespace TDEngine
{
	Clock::Clock()
	{
		setMode(HIGHRES);
	}


	float Clock::getElapsedTime()
	{
		if (mode == HIGHRES)
		{
			QueryPerformanceCounter(&finalTicks);
			return (finalTicks.QuadPart - initialTicks.QuadPart) * 1000.f / frequency.QuadPart;
		}
		else if (mode == STANDART)
			return clock.getElapsedTime().asMilliseconds();
	}


	void Clock::restart()
	{
		if (mode == HIGHRES)
		{
			QueryPerformanceFrequency(&frequency);
			QueryPerformanceCounter(&initialTicks);
		}
		else
			clock.restart();
	}


	void Clock::setMode(MODE m)
	{
		mode = m;
		restart();
	}
}