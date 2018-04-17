#pragma once
#include "RPGAttributes.h"

namespace TDGame
{
	class Item
	{
	public:
		enum ItemType {ANY, CHEST, HEAD, LEGGINS, ARM};

		Item(RPGAttributes Attrib, ItemType type, std::string name,
			std::string description) : AttribGain(Attrib), Type(type), Name(name), Description(description) {}
		virtual void OnUse() {}

		RPGAttributes AttribGain;

		const std::string& GetDescription() const {return Description;}
		const std::string& GetName()        const {return Name;}
		const ItemType& GetType()           const {return Type;}
	private:
		ItemType Type;
		std::string Name, Description;
	};
}