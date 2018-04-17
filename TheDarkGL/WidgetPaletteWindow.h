#pragma once
#include <SFGUI\Widgets.hpp>
#include <SFML/Graphics.hpp>

namespace TDGame
{
	class WidgetPaletteWindow : public sfg::Window
	{
	private:
		WidgetPaletteWindow();
	public:

		void Update();
		static std::shared_ptr<WidgetPaletteWindow> Create();
		const std::string& getBrush() const { return brush; }
	private:
		
		void OnClickPaletteButton();
		bool IsActive;
		std::vector<sf::RectangleShape*> tiles;
		std::string brush;
		std::shared_ptr<sfg::ToggleButton> brushButton;

		// Widgets
		sfg::Notebook::Ptr notebook;
		std::vector<std::pair<sfg::ToggleButton::Ptr, std::string > > Buttons;
	};
}