#pragma once
#include "EventManager.h"
#include <SFML\Graphics.hpp>

namespace TDEngine
{
	class Camera : EventReceiver
	{
	public:
		Camera();
		Camera(float, float, sf::Vector2f = sf::Vector2f(0.f, 0.f), sf::Vector2f = sf::Vector2f(0.f, 0.f), bool = true);

		virtual ~Camera();
		virtual void ProceedEvent(TDEngine::Event&);
		void Update();
		sf::View getView() const;
		sf::FloatRect getViewport() const;
		//returns transformed coordinates of point
		sf::Vector2f windowCoordToCamera(sf::Vector2f& windowPos, sf::Vector2f& windowSize) const;

		void setCameraSpeed(float s) { cameraSpeed = s; }
		void setZoomSpeed(float s) { zoomSpeed = s; }
		void activate()   { active = true; }
		void deactivate() { active = false; }
	private:
		float cameraSpeed;
		float zoomSpeed;
		float aspectFactor;
		sf::Vector2f position;
		//size of camera(field of view)
		sf::Vector2f size;
		//not moving and resizing when inactive 
		bool active;
	};
}

