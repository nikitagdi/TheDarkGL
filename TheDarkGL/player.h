#pragma once
#include "Entity.h"
#include <boost/serialization/serialization.hpp>

namespace TDGame
{

	class Player :
		public Entity
	{
		friend class boost::serialization::access;
	public:
		Player();
		Player(std::string, float X = 0.f, float Y = 0.f, unsigned int size =  8);
		virtual ~Player();
		
		virtual void Load()  {}
		virtual void Save()  {}
		virtual void ProceedEvent(TDEngine::Event&);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & boost::serialization::base_object<TDGame::Entity>(*this);
		}

	private:
		
	};
}

