#pragma once

namespace TDGame
{
	struct RPGAttributes
	{
		enum {STRENGHT = 0, AGILITY, INTELLIGENCE};
		// Amount of parameters gain per 

		static const unsigned int
			HPRatio = 10,
			MPRatio = 15,
			ASRatio = 1,
			MSRatio = 10,
			ArmorRatio = 2,
			AttackRatio = 1;

		static const unsigned int AmountOfAttributes = 3;

		RPGAttributes(int = 10, int = 10, int = 10, int = 100, int = 50,
			int = 20, int = 5, int = 30, int = 100);
		int GetHPGain();
		int GetMPGain();
		int GetASGain();
		int GetMSGain();
		int GetArmorGain();
		int GetAttackGain(); //depends on primal attribute

		int Attributes[AmountOfAttributes];
		int PrimalAttribute; // codes defined in enum
		int HP, MP, AttackDamage, Armor, AttackSpeed, MoveSpeed; // Base values for entities
	};
}