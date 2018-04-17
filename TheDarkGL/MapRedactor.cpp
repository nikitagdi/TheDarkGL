/*
 * MapRedactor.cpp - Code of the map editing tool
 * Version 1.0
 * Contains two windows used for redacting map elements: Window that changes parameters of tiles and window that contains all tiles in game.
 *
 * TODO:
 * Buttons for invoking two windows
 * Parameters window with textboxes for changing hp, passability and viewbox for id of tile
 * Palette window for chosing the tile
 * 
 *
 *
 *
 *
 *
 */


#include "MapRedactor.h"
#include "Game.h"
#include "Map.h"
#include <gl/GL.h>
#include "Utils.h"

extern TDEngine::GameManager TDEngine::GameEngine;


unsigned int sf::getTexId(sf::Texture* tex)
{
	return  tex->m_texture;
}

namespace TDGame
{
	MapRedactor::MapRedactor(Map*_map) : map(_map)  {}



	void MapRedactor::Render() {}

	void MapRedactor::UpdateMenu(float x, float y)
	{
		RedactorDesk = &TDEngine::GameEngine.GraphicsEngine.desktop;

		TDEngine::GameEngine.LogEngine.LogInfo("Loading palette widget", "MapRedactor");
		TDEngine::Clock clock;
		PaletteWindow = WidgetPaletteWindow::Create();
		PaletteWindow->Update();
		RedactorDesk->Add(PaletteWindow);
		TDEngine::GameEngine.LogEngine.LogInfo("Palette loading done in " 
			+ boost::lexical_cast<std::string>(clock.getElapsedTime()) + " ms", "MapRedactor");

		PropertiesWindow = WidgetPropertiesRedactor::Create();
		PropertiesWindow->Show(false);
		RedactorDesk->Add(PropertiesWindow);
		RedactorDesk->Add(PropertiesWindow->getTextureSelector());

	}


	void MapRedactor::ProceedEvent(sf::Event& event)
	{
		if (TDEngine::Utils::CheckMouseCollision(PaletteWindow, sf::Vector2f(event.mouseButton.x, event.mouseButton.y)) ||
			TDEngine::Utils::CheckMouseCollision(PropertiesWindow, sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
			return;

		sf::Vector2u windowsize = TDEngine::GameEngine.GraphicsEngine.Window->getSize();
		sf::Vector2f mouseClickPos = map->getCamera().windowCoordToCamera(sf::Vector2f(event.mouseButton.x, event.mouseButton.y), sf::Vector2f(windowsize.x, windowsize.y));

		switch (event.type)
		{
		case sf::Event::MouseButtonPressed:
			if (TDEngine::Utils::CheckMouseCollision(PaletteWindow, sf::Vector2f(event.mouseButton.x, event.mouseButton.y)) ||
				TDEngine::Utils::CheckMouseCollision(PropertiesWindow, sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
				break;

			if (event.mouseButton.button == sf::Mouse::Button::Left)
			{
				TerrainTile* tile = new TerrainTile(PaletteWindow->getBrush(), true, floor(mouseClickPos.x / 32.f)*32.f, floor(mouseClickPos.y / 32.f)*32.f, 13);
				tile->setTexture(*TDEngine::GameEngine.GraphicsEngine.GetTexture(PaletteWindow->getBrush()));
				Map::MapInstance->addObject(*tile);
			}
			break;

		case sf::Event::MouseButtonReleased:
			if (TDEngine::Utils::CheckMouseCollision(PaletteWindow, sf::Vector2f(event.mouseButton.x, event.mouseButton.y)) ||
				TDEngine::Utils::CheckMouseCollision(PropertiesWindow, sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
				break;
			if (event.mouseButton.button == sf::Mouse::Button::Right)
					PropertiesWindow->Update(Map::MapInstance->getObjectsByMouseCoordinates(event.mouseButton.x, event.mouseButton.y));
			break;
		}
	}

}