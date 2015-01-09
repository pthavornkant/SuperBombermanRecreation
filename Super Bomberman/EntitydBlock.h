#pragma once

#include "Entity.h"
#include <iostream>

class EntitydBlock : public Entity
{
	static char animDestIdle[];
	static char animDestBreak[];
public:
	EntitydBlock(void);
	EntitydBlock(float x, float y);
	~EntitydBlock(void);
	void update();
	void destruct();
	//Test
};

