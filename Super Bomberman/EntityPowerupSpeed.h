#pragma once

#include "Entity.h"
#include <iostream>

class EntityPowerupSpeed : public Entity
{
	static char animPowerupSpeedIdle[];

	//int fireLength; //Length of the fire. Supposed to be a variable because power ups should be able to make the fire bigger.
public:
	static char ENTTYPE;

	EntityPowerupSpeed(void);
	EntityPowerupSpeed(float x, float y);
	~EntityPowerupSpeed(void);
};

