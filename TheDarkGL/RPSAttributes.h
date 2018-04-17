#pragma once
#include <boost/serialization/serialization.hpp>

namespace TDGame
{
	class RPSAttributes
	{
		friend class boost::serialization::access;
	public:
		RPSAttributes();
		RPSAttributes(float maxHP, float maxSpeed);
		~RPSAttributes();

		RPSAttributes operator+(const RPSAttributes&) const;

		//resets all values to the maximum values
		void reset();

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & MaxHealthPoints;
			ar & MaxSpeed;
			ar & HealthPoints;
		}

		//Maximum values
		float MaxHealthPoints;
		float MaxSpeed;

		//Current values
		float HealthPoints;
	};
}

