#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFGUI\Widgets.hpp>
#include "Tile.h"
#include "WidgetTextureSelector.h"


namespace TDGame
{
	class WidgetPropertiesRedactor : public sfg::Window
	{
	private:
		WidgetPropertiesRedactor();
	public:

		void Update(std::vector<Tile*>&);
		static std::shared_ptr<WidgetPropertiesRedactor> Create();

		const std::shared_ptr< WidgetTextureSelector >& getTextureSelector() const
		{
			return texSelector;
		}

	private:
		std::shared_ptr < sfg::Box > box; // container for widgets
		std::shared_ptr < sfg::Label > IDlabel;
		std::shared_ptr < sfg::Box > passabilityBox;
		std::shared_ptr < sfg::Label > passabilityLabel;
		std::shared_ptr < sfg::ComboBox > passabilityCombobox;
		
		std::shared_ptr < sfg::Label > tileImageLabel;
		std::shared_ptr < sfg::Image > tileImage;

		//combobox with available tiles
		std::shared_ptr < sfg::ComboBox > tileListCombobox; 
		std::shared_ptr < sfg::Button > applyButton;

		std::shared_ptr< WidgetTextureSelector > texSelector;

		void OnCloseClick();
		void OnApplyClick();
		void OnTileListSelect();
		void OnTileImageClick();
		void OnNewTextureSelected(sf::Texture*);

		Tile* CurrentTile;

		//Storage for all tiles at position
		std::vector<Tile*> tiles;
	};
}