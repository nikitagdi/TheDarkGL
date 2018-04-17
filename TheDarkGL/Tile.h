#pragma once;
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "AnimationManager.h"
#include <string>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
//Tile.h Definition of render information


namespace boost {
	namespace serialization {

		template<class Archive>
		void serialize(Archive & ar, sf::Sprite & g, const unsigned int version)
		{
			ar & boost::serialization::base_object<sf::Drawable>(g);
			ar & boost::serialization::base_object<sf::Transformable>(g);
		}

		template<class Archive>
		void serialize(Archive & ar, sf::Drawable & g, const unsigned int version){}

		template<class Archive>
		void serialize(Archive & ar, sf::Transformable & g, const unsigned int version){}

	} // namespace serialization
} // namespace boost


namespace TDGame
{
	class Map;

	class Tile : public sf::Sprite, 
				 public TDEngine::EventReceiver
	{
		friend class boost::serialization::access;
	public:
		Tile(std::string = "DEFAULT NAME", float X = 0.f, float Y = 0.f, unsigned int size = 16, Map* map = nullptr);
		virtual ~Tile() = 0 {}
		virtual void ProceedEvent(TDEngine::Event&);
		bool Tile::operator==(const Tile &b) const
		{
			if (Name == b.GetName() &&
				Size == b.GetSize() &&
				getTexture() == b.getTexture() &&
				getGlobalBounds() == b.getGlobalBounds())
				return true;
			return false;
		}

		std::string GetName() const;
		int GetSize() const;
		const TDEngine::AnimationManager& getAnimations() const { return animations; }
		void SetSize(int size) { Size = size; }
		void SetName(std::string);
		void SetAnimation(unsigned int id, TDEngine::Animation&);

		//---------------------------------------------------------
		//Serialization routine
		template<class Archive>
		void save(Archive & ar, const unsigned int version) const
		{
			ar << boost::serialization::base_object<sf::Sprite>(*this);
			ar << boost::serialization::base_object<TDEngine::EventReceiver>(*this);
			ar << Name;
			ar << Size;
			ar << TDEngine::GameEngine.GraphicsEngine.GetTextureName(getTexture());
			ar << getPosition().x;
			ar << getPosition().y;
		}
		template<class Archive>
		void load(Archive & ar, const unsigned int version)
		{
			std::string tex;
			float x = 0.f, y = 0.f;


			ar >> boost::serialization::base_object<sf::Sprite>(*this);
			ar >> boost::serialization::base_object<TDEngine::EventReceiver>(*this);
			ar >> Name;
			ar >> Size;
			ar >> tex;
			ar >> x;
			ar >> y;

			setPosition(sf::Vector2f(x, y));
			setTexture(*TDEngine::GameEngine.GraphicsEngine.GetTexture(tex));
			void updateGlobalBounds();
		}
		BOOST_SERIALIZATION_SPLIT_MEMBER()
		//----------------------------------------------------------

		void setTexture(const sf::Texture& texture, bool resetRect = false);
		void setMap(Map* map);
		sf::Vector2f getActualSize() const;
		sf::FloatRect getGlobalBounds() const; //{return globalBounds;}
		void updateGlobalBounds()  {globalBounds = sf::Sprite::getGlobalBounds();}
	private:
		std::string Name;
		int Size;
		sf::FloatRect globalBounds;
	protected:
		//texture of an object without animation
		const sf::Texture* defaultTexture;
		TDEngine::AnimationManager animations;
		//The map tile belongs to
		Map* world;
		virtual void Update(float ms);
	};
}