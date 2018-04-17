#include "RPSAttributes.h"

namespace TDGame
{

	RPSAttributes::RPSAttributes()
	{
		MaxHealthPoints =
			MaxSpeed =
			HealthPoints = 0;
	}


	RPSAttributes::RPSAttributes(float maxHP, float maxSpeed)
	{
		MaxHealthPoints = maxHP;
		MaxSpeed = maxSpeed;
		HealthPoints = 0;
	}


	RPSAttributes::~RPSAttributes()
	{
	}


	void RPSAttributes::reset()
	{
		HealthPoints = MaxHealthPoints;
	}


	RPSAttributes RPSAttributes::operator+(const RPSAttributes& param) const
	{
		RPSAttributes ret;
		ret.MaxHealthPoints = param.MaxHealthPoints + this->MaxHealthPoints;
		ret.MaxSpeed = param.MaxSpeed + this->MaxSpeed;
		ret.HealthPoints = param.HealthPoints + this->HealthPoints;

		return ret;
	}

}