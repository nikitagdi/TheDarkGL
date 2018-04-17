#pragma once
#include "Item.h"
#include <vector>

namespace TDGame
{
	struct Slot
	{
		Item* item;
		Item::ItemType SlotType;
	};


	class Inventory
	{
	public:
		Inventory();

		bool AddItem(Item* , int );
		bool AddItem(Item*);
		bool DeleteItem(Item* , int );
		bool DeleteItem(Item*);
		Item* GetItem(int);

		const std::vector<Slot>& GetItems() const {return Items;}

	private:
		std::vector<Slot> Items;
	};
}