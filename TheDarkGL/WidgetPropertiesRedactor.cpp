#include "WidgetPropertiesRedactor.h"
#include "Terrain.h"
#include "Utils.h"

namespace TDGame
{
	WidgetPropertiesRedactor::WidgetPropertiesRedactor() : sfg::Window(sfg::Window::Style::TITLEBAR | sfg::Window::Style::BACKGROUND | sfg::Window::Style::RESIZE | sfg::Window::Style::CLOSE)
	{
		SetTitle("Change properties");
		this->GetSignal(sfg::Window::OnCloseButton).Connect(std::bind(&WidgetPropertiesRedactor::OnCloseClick, this));
		texSelector = WidgetTextureSelector::Create();
		texSelector->Update();
		texSelector->Show(false);
		std::function<void(sf::Texture*)> func = std::bind(&WidgetPropertiesRedactor::OnNewTextureSelected, this,
			std::placeholders::_1);
		texSelector->registerCallback(func);
	}


	std::shared_ptr<WidgetPropertiesRedactor> WidgetPropertiesRedactor::Create()
	{
		std::shared_ptr<WidgetPropertiesRedactor> widget(new WidgetPropertiesRedactor());
		return widget;
	}


	void WidgetPropertiesRedactor::Update(std::vector<Tile*>& _tiles)
	{
		if (_tiles.empty())
			return;

		tiles = _tiles;

		CurrentTile = tiles.at(0);

		Show(true);
		auto ScreenSize = TDEngine::GameEngine.GraphicsEngine.Window->getSize();
		this->Remove(box);

		box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
		box->SetSpacing(5.f);
		//box->set
		//box->SetRowSpacings(5.f);

		Add(box);

		
		
		//if (!this->GetParent()->IsChild(texSelector))
		//	this->GetParent()->Add(texSelector);

		tileListCombobox = sfg::ComboBox::Create();
		for (auto tile : tiles)
			tileListCombobox->AppendItem(tile->GetName());
		tileListCombobox->SelectItem(0);
		tileListCombobox->GetSignal(sfg::ComboBox::OnSelect).Connect(std::bind(&WidgetPropertiesRedactor::OnTileListSelect, this));
		box->Pack(tileListCombobox);

		box->SetRequisition(sf::Vector2f(150.f, 0.f));

		std::shared_ptr < sfg::Box > imageBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
		tileImageLabel = sfg::Label::Create("Tile texture ");
		tileImage = sfg::Image::Create(CurrentTile->getTexture()->copyToImage());
		tileImage->GetSignal(sfg::Image::OnLeftClick).Connect(std::bind(&WidgetPropertiesRedactor::OnTileImageClick, this));
		imageBox->Pack(tileImageLabel);
		imageBox->Pack(tileImage);
		box->Pack(imageBox);

		auto terrainTile = dynamic_cast<TerrainTile*>(CurrentTile);
		if (terrainTile)
		{

			IDlabel = sfg::Label::Create(" ID: " + boost::lexical_cast<std::string>(terrainTile->getID()));
			box->Pack(IDlabel, sfg::Table::FILL);

			passabilityBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
			passabilityLabel = sfg::Label::Create(" Passable:" + terrainTile->IsPassable());
			passabilityCombobox = sfg::ComboBox::Create();
			passabilityCombobox->AppendItem("false");
			passabilityCombobox->AppendItem("true");
			passabilityCombobox->SelectItem(terrainTile->IsPassable());
			passabilityBox->Pack(passabilityLabel);
			passabilityBox->Pack(passabilityCombobox);
			box->Pack(passabilityBox, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

		}

		applyButton = sfg::Button::Create("Apply");
		applyButton->GetSignal(sfg::Window::OnLeftClick).Connect(std::bind(&WidgetPropertiesRedactor::OnApplyClick, this));
		box->Pack(applyButton, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

		SetPosition(sf::Vector2f((ScreenSize.x / 2) - GetAllocation().width, (ScreenSize.y / 2) - GetAllocation().height));

	}


	void WidgetPropertiesRedactor::OnTileListSelect()
	{
		try {
			CurrentTile = tiles.at(tileListCombobox->GetSelectedItem());
		}
		catch (std::out_of_range& e)
		{
			TDEngine::GameEngine.LogEngine.LogError(e.what(), "Properties Widget");
			return;
		}

		auto terrainTile = dynamic_cast<TerrainTile*>(CurrentTile);
		if (terrainTile)
			passabilityCombobox->SelectItem(terrainTile->IsPassable());

		tileImage->SetImage(TDEngine::Utils::ResizeImage(*(CurrentTile->getTexture()), 32, 32));
		
	}


	void WidgetPropertiesRedactor::OnCloseClick()
	{
		Show(false);
	}


	void WidgetPropertiesRedactor::OnApplyClick()
	{
		if (!CurrentTile)
			return;

		auto terrainTile = dynamic_cast<TerrainTile*>(CurrentTile);
		if (terrainTile)
		{
			terrainTile->SetPassability(passabilityCombobox->GetSelectedItem());
		}

		this->Show(false);
	}

	void WidgetPropertiesRedactor::OnTileImageClick()
	{
		//texSelector->Update();
		texSelector->Show(true);
	}
	

	void WidgetPropertiesRedactor::OnNewTextureSelected(sf::Texture* tex)
	{
		if (CurrentTile == nullptr || tex == nullptr)
			return;

		CurrentTile->setTexture(*tex);
		texSelector->Show(false);
		Update(tiles);
	}
}