#include "Camera.h"
#include "Game.h"

namespace TDEngine
{
	extern GameManager GameEngine;

	Camera::Camera()
	{
		cameraSpeed = TDEngine::GameEngine.GetSetting<float>("CameraSpeed", "20");
		zoomSpeed   = TDEngine::GameEngine.GetSetting<float>("ZoomSpeed", "30");
		size        = sf::Vector2f(GameEngine.GraphicsEngine.Window->getSize().x, GameEngine.GraphicsEngine.Window->getSize().y);
		position    = size / 2.f;
		active      = true;
		aspectFactor = 1.f;
		GameEngine.EventEngine.Subscribe(this, "event_mouseWheelTurned");
	}


	Camera::Camera(float s, float zs, sf::Vector2f pos, sf::Vector2f _size, bool a) : cameraSpeed(s), zoomSpeed(zs), position(pos), size(_size), active(a)
	{
		aspectFactor = 1.f;
		GameEngine.EventEngine.Subscribe(this, "event_mouseWheelTurned");
	}


	Camera::~Camera()
	{

	}


	sf::View Camera::getView() const
	{
		return sf::View(position, size*aspectFactor);
	}


	sf::FloatRect Camera::getViewport() const
	{
		return sf::FloatRect(position - (size*aspectFactor / 2.f), size*aspectFactor);
	}


	void Camera::Update()
	{
		sf::RenderWindow* wnd = TDEngine::GameEngine.GraphicsEngine.Window;
		int width = wnd->getView().getSize().x;
		int height = wnd->getView().getSize().y;
		sf::Vector2i mousePos = sf::Mouse::getPosition(*wnd);
		sf::Vector2u windowsize = wnd->getSize();
		float cameraDisplacement = cameraSpeed * GameEngine.getElapsedTime() / 1000.f;

		if ((mousePos.x <= width + 10) && (mousePos.x >= width - 2))
			position.x += cameraDisplacement;
		if (position.x > width / 2)
			if ((mousePos.x >= -10) && (mousePos.x <= 2))
				position.x -= cameraDisplacement;
		if ((mousePos.y <= height + 10) && (mousePos.y >= height - 2))
			position.y += cameraDisplacement;
		if (position.y > height / 2)
			if ((mousePos.y >= -30) && (mousePos.y <= 2))
				position.y -= cameraDisplacement;

		if (position.y < height / 2)
			position.y = height / 2;
		if (position.x < width / 2)
			position.x = width / 2;
	}


	void Camera::ProceedEvent(TDEngine::Event& ev)
	{
		if (ev.name == "event_mouseWheelTurned")
		{
			int delta = *((int*)ev.Parameters[0].second);
			if (delta > 0)
				aspectFactor += zoomSpeed;
			else if (delta < 0)
				aspectFactor -= zoomSpeed;
		}

		else if (ev.name == "event_buttonPressed")
		{

		}
	}


	sf::Vector2f Camera::windowCoordToCamera(sf::Vector2f& windowPos, sf::Vector2f& windowSize) const
	{
		return (windowPos*aspectFactor) + (position - (size*aspectFactor / 2.f));
	}
}