#pragma once

#include "Entity.h"
#include "Character.h"
#include <iostream>

class EntityBomb : public Entity
{
	Character* owner;
	static char animBombIdle[];
	static int FUSETIME;
	static int DEFAULTBLASTRADIUS;
	int fuse;
	int blastRadius;

public:
	EntityBomb(void);
	EntityBomb(float x, float y);
	EntityBomb(float x, float y, Character* owner, int blastRad);
	~EntityBomb(void);
	virtual void update();
	void destruct();
	int getBlastRadius();
};

