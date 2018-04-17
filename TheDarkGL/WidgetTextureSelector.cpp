#include "WidgetTextureSelector.h"
#include "Game.h"
#include "Utils.h"

namespace TDGame
{

	WidgetTextureSelector::WidgetTextureSelector() : sfg::Window(sfg::Window::Style::TITLEBAR)
	{

	}


	WidgetTextureSelector::~WidgetTextureSelector()
	{
	}


	void WidgetTextureSelector::Update()
	{
		auto ScreenSize = TDEngine::GameEngine.GraphicsEngine.Window->getSize();
		int i = 0, j = 0, tmp = 0;
		auto RedactorDesk = &TDEngine::GameEngine.GraphicsEngine.desktop;
		this->SetRequisition(sf::Vector2f(300.f, 400.f));
		this->SetStyle(sfg::Window::Style::TITLEBAR); // Remove title bar, make window unresizable and unmovable
		this->SetTitle("Select texture");

		notebook = sfg::Notebook::Create();
		notebook->SetScrollable(true);
		notebook->SetRequisition(sf::Vector2f(300.f, 0.f));

		Buttons.clear();

		for (auto tileset : TDEngine::GameEngine.GraphicsEngine.getTilesets())
		{
			auto box = sfg::Table::Create();
			auto scrolledWnd = sfg::ScrolledWindow::Create();
			scrolledWnd->SetScrollbarPolicy(sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC
				| sfg::ScrolledWindow::VERTICAL_AUTOMATIC); // Remove scrollbars when they are unnessesary
			scrolledWnd->AddWithViewport(box);
			notebook->AppendPage(scrolledWnd, sfg::Label::Create(tileset.name));

			for (auto tile : tileset.tiles)
			{
				tmp++;
				if (tile.find("Animation") != std::string::npos)
					continue;
				auto buttonTexture = sfg::Image::Create();

				auto texture = TDEngine::GameEngine.GraphicsEngine.GetTexture(tile);
				if ((texture->getSize().x != 32) ||
					(texture->getSize().y != 32))
					buttonTexture->SetImage(TDEngine::Utils::ResizeImage(*texture, 32, 32));
				else
					buttonTexture->SetImage(texture->copyToImage());

				auto button = sfg::Button::Create();
				button->SetImage(buttonTexture);
				button->SetAllocation(sf::FloatRect(button->GetAllocation().left, button->GetAllocation().top, 32.f, 32.f));
				button->GetSignal(sfg::Button::OnMouseLeftPress).Connect(std::bind(&WidgetTextureSelector::OnClickPaletteButton, this));

				box->Attach(button, sf::Rect<sf::Uint32>(i, j, 1, 1));
				Buttons.push_back(std::pair<std::shared_ptr<sfg::Button>, std::string >(button, tile));
				i++;
				if (i > (int)(300/32))
				{
					i = 0;
					j++;
				}
			}
			i = j = 0;
		}

		this->Add(notebook);
		this->SetPosition(sf::Vector2f(ScreenSize.x - this->GetAllocation().width, 0.f));

		TDEngine::GameEngine.LogEngine.LogInfo("Totally " + boost::lexical_cast<std::string>(tmp) + "Textures were resized");
	}

	void WidgetTextureSelector::registerCallback(std::function<void(sf::Texture*)> func)
	{
		callback = func;
	}


	void WidgetTextureSelector::OnClickPaletteButton()
	{
		std::vector<std::pair<std::shared_ptr<sfg::Button>, std::string > >::iterator it = std::find_if(Buttons.begin(), Buttons.end(),
			[](const std::pair<std::shared_ptr<sfg::Button>, std::string >& s) { return s.first->GetState() == sfg::Widget::State::ACTIVE; });

		if (it != Buttons.end())
		{
			callback(TDEngine::GameEngine.GraphicsEngine.GetTexture((*it).second));
		}
	}


	std::shared_ptr<WidgetTextureSelector> WidgetTextureSelector::Create()
	{
		std::shared_ptr<WidgetTextureSelector> widget(new WidgetTextureSelector());
		return widget;
	}
}