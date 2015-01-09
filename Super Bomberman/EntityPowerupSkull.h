#pragma once

#include "Entity.h"
#include <iostream>

class EntityPowerupSkull : public Entity
{
	static char animPowerupSkullIdle[];

	//int fireLength; //Length of the fire. Supposed to be a variable because power ups should be able to make the fire bigger.
public:
	static char ENTTYPE;

	EntityPowerupSkull(void);
	EntityPowerupSkull(float x, float y);
	~EntityPowerupSkull(void);
};

