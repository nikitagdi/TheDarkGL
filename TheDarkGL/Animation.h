#pragma once
#include <SFML/Graphics.hpp>

namespace TDEngine
{
	class Animation
	{
	public:
		static const int MAXFRAMES = 16;
		//Constructor
		//Arguments: pointer to array, size of the array, duration of animation in seconds, default texture of animation
		Animation(sf::Texture*[], unsigned int, float, sf::Texture&);
		Animation() {}

		const sf::Texture* getCurrentFrame() const;
		bool isFinished() const;
		//Updates animation according to time elapsed since last update
		//Arguments: time in milliseconds
		void Update(float);
		//Restarts animation
		void Restart();
		//Force animation to finish
		void Finish();
	private:
		//Frames container
		const sf::Texture* frames[MAXFRAMES];
		float duration;
		//index of current frame
		unsigned int currentFrame;
		float currentTime;
		//total number of frames for animation
		unsigned int totalFrameNumber;
		//Frame when animation inactive
		sf::Texture* defaultFrame;
		//Has the animation finished
		bool finished;
	};

}