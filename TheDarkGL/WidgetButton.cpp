#include "WidgetButton.h"

namespace TDEngine
{
	void WidgetButton::MouseEvent(sf::Event& event)
	{
		if(event.type == sf::Event::MouseButtonPressed)
		{
			Pressed = true;
		}
		else if(event.type == sf::Event::MouseButtonReleased)
		{
			Pressed = false;
		}
	}
}