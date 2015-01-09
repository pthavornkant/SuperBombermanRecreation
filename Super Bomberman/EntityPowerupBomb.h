#pragma once

#include "Entity.h"
#include <iostream>

class EntityPowerupBomb : public Entity
{
	static char animPowerupBombIdle[];

	//int fireLength; //Length of the fire. Supposed to be a variable because power ups should be able to make the fire bigger.
public:
	static char ENTTYPE;

	EntityPowerupBomb(void);
	EntityPowerupBomb(float x, float y);
	~EntityPowerupBomb(void);
};

