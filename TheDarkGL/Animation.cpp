#include "Animation.h"

namespace TDEngine
{
	Animation::Animation(sf::Texture* arr[], unsigned int size, float _duration, sf::Texture& defaultF)
		: totalFrameNumber(size), duration(_duration * 1000.f), currentFrame(0), currentTime(0.f), finished(true)
		, defaultFrame(&defaultF)
	{
		for (unsigned int i = 0; i < size; i++)
			frames[i] = arr[i];
	}


	const sf::Texture* Animation::getCurrentFrame() const
	{
		if (finished)
			return defaultFrame;
		return frames[currentFrame];
	}


	bool Animation::isFinished() const
	{
		return finished;
	}


	void Animation::Restart()
	{
		finished = false;
	}


	void Animation::Update(float timeElapsed)
	{
		if (finished)
			return;

		const int oneFrameTime = static_cast<float>(totalFrameNumber) / duration;
		currentTime += timeElapsed;

		if (currentTime >= duration)
		{
			Finish();
			return;
		}

		currentFrame = static_cast<unsigned int>(currentTime / (duration / totalFrameNumber));
	}


	void Animation::Finish()
	{
			currentTime = 0.f;
			currentFrame = 0;
			finished = true;
	}
}