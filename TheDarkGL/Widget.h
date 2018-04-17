// Widget.h - base abstract class for all graphical 
// widgets in the game

#pragma once
#include "Game.h"
#include <SFML/Graphics.hpp>

namespace TDEngine
{
	class Widget
	{
	public:
		Widget(sf::Vector2i position, sf::Vector2i size, sf::Texture* texture, bool active = true,
			bool visible = true) : Position(position), Size(size), Texture(texture), Active(active), Visible(visible)
		{
			DrawRect.setSize(sf::Vector2f(Size.x, Size.y));
			DrawRect.setPosition(sf::Vector2f(Position.x, Position.y));
			DrawRect.setTexture(Texture);
		}

		// Called when mouse pressed widget
		virtual void MouseEvent(sf::Event&) = 0;
		virtual void Render()
		{
			if(Visible && Active)
				GameEngine.GraphicsEngine.Window->draw(DrawRect);
		}
		// Checks if widget contains mouse
		virtual bool Collide(sf::Vector2i MousePos) 
		{
			sf::Rect<int> ObjCBox; ObjCBox.left = Size.x;
			ObjCBox.top = Size.y; 
			ObjCBox.width = Size.x+Position.x;
			ObjCBox.height = Size.y+Position.y;
			return ObjCBox.contains(MousePos);
		}

		bool IsActive() {return Active;}
		bool IsVisible() {return Visible;}
		void SetActive(bool active)  {Active = active;}
		void SetVisible(bool visible){Visible = visible;}
		sf::Vector2i GetPosition() {return Position;}
		sf::Vector2i GetSize() {return Size;}
		sf::Texture* GetTexture() {return Texture;}
		void SetTexture(sf::Texture* tex) {Texture = tex;DrawRect.setTexture(Texture);}	
		const std::string& GetName() const {return name;} 

	private:
		bool Active, Visible;
		sf::Vector2i Position, Size;
		sf::Texture* Texture;
		sf::RectangleShape DrawRect;
		std::string name;
	};
}