#pragma once

#include "Entity.h"
#include <iostream>

class EntityFire : public Entity
{
	static char animFireCenterIdle[], animFireHorizIdle[], animFireVertIdle[], animFireTopIdle[], animFireBottomIdle[], animFireLeftIdle[], animFireRightIdle[];

	//int fireLength; //Length of the fire. Supposed to be a variable because power ups should be able to make the fire bigger.
public:
	EntityFire(void);
	EntityFire(float x, float y);
	EntityFire(float x, float y, char dir);
	~EntityFire(void);
	void update();

	char direction;
	enum {
		FIREDIR_CENTER = 0,
		FIREDIR_HORIZ = 1,
		FIREDIR_VERT = 2,
		FIREDIR_TOP = 3,
		FIREDIR_BOTTOM = 4,
		FIREDIR_LEFT = 5,
		FIREDIR_RIGHT = 6
	};

	void setDirection(char dir);
};

