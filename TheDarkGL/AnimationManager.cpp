#include "AnimationManager.h"
#include "Game.h"

extern TDEngine::GameManager TDEngine::GameEngine;

namespace TDEngine
{
	AnimationManager::AnimationManager() : currentAnimation(0), active(true)
	{}


	Animation& AnimationManager::GetCurrentAnimation() 
	{
		return GetAnimation(currentAnimation);
	}


	Animation& AnimationManager::GetAnimation(int id) 
	{
		try
		{
			return animationsContainer.at(id);
		}
		catch (std::out_of_range&)
		{
			GameEngine.LogEngine.LogError("Animation with index " + boost::lexical_cast<std::string>(id) + std::string(" is not found"), "AnimationManager");
			return GameEngine.GraphicsEngine.GetAnimation("Default");
		}
	}


	void AnimationManager::Update(float ms)
	{
		if (animationsContainer.empty())
			return;
		if (!active)
			return;
		Animation& currAnim = GetCurrentAnimation();
		if (currAnim.isFinished())
			currAnim.Restart();

		currAnim.Update(ms);
	}


	void AnimationManager::RegisterAnimation(int id, Animation& anim)
	{
		animationsContainer[id] = anim;
	}


	void AnimationManager::ChangeState(int id)
	{
		currentAnimation = id;

		//GetCurrentAnimation().Restart();
	}


	const sf::Texture* AnimationManager::GetCurrentFrame() 
	{
		if (animationsContainer.empty())
			return nullptr;
		return GetCurrentAnimation().getCurrentFrame();
	}


	void AnimationManager::Activate()
	{
		active = true;
		GetCurrentAnimation().Restart();
	}


	void AnimationManager::Deactivate()
	{
		active = false;
		GetCurrentAnimation().Finish();
	}
}