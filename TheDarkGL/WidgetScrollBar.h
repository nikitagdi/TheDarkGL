#pragma once
#include "WidgetButton.h"

namespace TDEngine
{

	class WidgetScrollBar : public Widget
	{
	public:
		WidgetScrollBar(sf::Vector2i position, sf::Vector2i size, sf::Texture* texture, bool active = true,
			bool visible = true) : Widget(position, size, texture, active, visible) {}

		bool IsPressed() {return Pressed;}
		void SetPressed(bool pressed) { Pressed = pressed;}
		virtual void MouseEvent(sf::Event&) override;
	private:
		bool Pressed;
	};
}