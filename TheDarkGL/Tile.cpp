#include "Tile.h"

namespace TDGame
{
	Tile::Tile(std::string Name, float X, float Y, unsigned int size, Map* map)
	{
		setPosition(sf::Vector2f(X, Y));
		Size = size;
		this->Name = Name;
		world = map;
		updateGlobalBounds();
		TDEngine::GameEngine.EventEngine.Subscribe(this, "event_Update");
	}


	std::string Tile::GetName() const
	{
		return this->Name;
	}


	int Tile::GetSize() const
	{
		return Size;
	}


	void Tile::SetName(std::string name)
	{
		this->Name = name;
	}

	void Tile::setTexture(const sf::Texture& texture, bool resetRect)
	{
		defaultTexture = &texture;
		sf::Sprite::setTexture(texture, resetRect);
		setScale((float)Size/texture.getSize().x, (float)Size/texture.getSize().y);
		updateGlobalBounds();
	}

	sf::Vector2f Tile::getActualSize() const
	{
		return sf::Vector2f(getGlobalBounds().width, getGlobalBounds().height);
	}

	sf::FloatRect Tile::getGlobalBounds() const
	{
		sf::Rect<float> ObjCBox;
		ObjCBox.left = getPosition().x;
		ObjCBox.top = getPosition().y; 
		ObjCBox.width = Sprite::getGlobalBounds().width;
		ObjCBox.height = Sprite::getGlobalBounds().height;
		return ObjCBox;
	}

	void Tile::ProceedEvent(TDEngine::Event& event)
	{

		if (event.name.compare("event_Update") == 0)
		{
			Tile::Update(*((float*)event.Parameters[0].second));
		}

	}


	void Tile::Update(float ms)
	{
			animations.Update(ms);

			const sf::Texture* newTexture = animations.GetCurrentFrame();
			if(newTexture)
				if (newTexture != getTexture())
				{
					sf::Sprite::setTexture(*newTexture, true);
					setScale((float)GetSize() / newTexture->getSize().x, (float)GetSize() / newTexture->getSize().y);
					updateGlobalBounds();
				}
	}


	void Tile::SetAnimation(unsigned int id, TDEngine::Animation& anim)
	{
		animations.RegisterAnimation(id, anim);
	}

	
	void Tile::setMap(Map* map)
	{
		world = map;
	}
}