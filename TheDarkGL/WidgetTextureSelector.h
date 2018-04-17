#pragma once
#include <SFGUI\Widgets.hpp>

namespace TDGame
{
	class WidgetTextureSelector : public sfg::Window
	{
	public:
		WidgetTextureSelector();
		~WidgetTextureSelector();


		void Update();

		void registerCallback(std::function<void(sf::Texture*)>);

		static std::shared_ptr<WidgetTextureSelector> Create();

	private:
		void OnClickPaletteButton();

		//callback, that is called, when texture has been selected
		std::function<void(sf::Texture*)> callback;

		// Widgets
		sfg::Notebook::Ptr notebook;
		std::vector<std::pair<sfg::Button::Ptr, std::string > > Buttons;
	};
}
