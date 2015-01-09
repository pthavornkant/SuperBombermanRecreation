#include "EntityPowerupBomb.h"
#include "World.h"

char EntityPowerupBomb::animPowerupBombIdle[] = {2,1,0,0,1,0,-1};
char EntityPowerupBomb::ENTTYPE = 2;

EntityPowerupBomb::EntityPowerupBomb(void) : Entity(1, 1) { 
	cout << "Warning! EntityPowerupBomb::EntityPowerupBomb() Default Constructor Used" << endl;
	centerX = 8.0f;
	centerY = 8.0f;
	width = 16.0f;
	height = 16.0f;
	setTexture("textures/default/ent/pwrup.png");
	setAnimation(animPowerupBombIdle);
	mapDataMask = World::MAPDATA_ENTITY;
	entType = ENTTYPE;
}

EntityPowerupBomb::EntityPowerupBomb(float x, float y) : Entity(x, y) { 
	centerX = 8.0f;
	centerY = 8.0f;
	width = 16.0f;
	height = 16.0f;
	setTexture("textures/default/ent/pwrup.png");
	setAnimation(animPowerupBombIdle);
	mapDataMask = World::MAPDATA_ENTITY;
	entType = ENTTYPE;
}

EntityPowerupBomb::~EntityPowerupBomb(void) {
	//cout << "EntityFlamePower destructor" << endl;
}