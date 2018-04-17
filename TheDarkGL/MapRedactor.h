#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFGUI\Widgets.hpp>
#include "Tile.h"
#include "WidgetPropertiesRedactor.h"
#include "WidgetPaletteWindow.h"
#include "Map.h"

namespace TDGame
{


	class MapRedactor
	{
	public:
		MapRedactor(Map* = 0);
		void ProceedEvent(sf::Event&);
		void Render();
		//Updates position and size of menu
		//x, y size of menu tiles. default is (128, 128)
		void UpdateMenu(float = 128.f, float = 128.f); 
		void setMap(Map* _map) { map = _map; }

		sf::FloatRect GetMenuBounds() const;
		float pos, sizeX, sizeY;
		bool IsRedactorActive()      const {return IsActive;}
	private:
		void OnClickPaletteButton();

		bool IsActive;

		std::shared_ptr<WidgetPropertiesRedactor>  PropertiesWindow;
		std::shared_ptr<WidgetPaletteWindow>  PaletteWindow;
		sfg::Desktop* RedactorDesk;
		Map* map; //Map that is currently being redacted
	};
}