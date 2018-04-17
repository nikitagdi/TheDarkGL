#pragma once
#include "Tile.h"
#include <boost/serialization/serialization.hpp>


namespace TDGame
{
	class TerrainTile : public Tile
	{
		friend class boost::serialization::access;
	public:
		TerrainTile(std::string, bool, float, float, int);
		TerrainTile() : Tile(), id(0) {}

		bool IsPassable();
		int getID() const {return id;}
		void setID(int ID) { id = ID;}
		void SetPassability(bool flag) {Passable = flag;}
		virtual void ProceedEvent(TDEngine::Event&);
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & boost::serialization::base_object<Tile>(*this);
			ar & Passable;
			ar & id;
		}
		virtual void Load() {}
		virtual void Save() {}

	private:
		bool Passable;
		int id;
	};
}
