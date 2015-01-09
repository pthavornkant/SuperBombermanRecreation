#pragma once

#include "Entity.h"
#include <iostream>

class EntityPowerupBlast : public Entity
{
	static char animPowerupBlastIdle[];

	//int fireLength; //Length of the fire. Supposed to be a variable because power ups should be able to make the fire bigger.
public:
	static char ENTTYPE;

	EntityPowerupBlast(void);
	EntityPowerupBlast(float x, float y);
	~EntityPowerupBlast(void);
};

