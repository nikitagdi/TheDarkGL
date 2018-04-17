#pragma once
#include "Animation.h"

namespace TDEngine
{
	class AnimationManager
	{
	public:
		AnimationManager();
		//Registers animation with given identificator
		void RegisterAnimation(int, Animation&);

		//Wrapper for Animation::Update(float ms), float is time in milliseconds
		void Update(float);
		//Changes current animation
		void ChangeState(int);

		//Make manager active(animation are updating)
		void Activate();
		//Make manager inactive(animation are not updating)
		void Deactivate();
		bool IsActive() const { return active; }

		Animation& GetCurrentAnimation();
		Animation& GetAnimation(int);
		const sf::Texture* GetCurrentFrame();
	private:
		std::map<int, Animation> animationsContainer;
		int currentAnimation;
		bool active;

	};
}