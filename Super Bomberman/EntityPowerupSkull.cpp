#include "EntityPowerupSkull.h"
#include "World.h"

char EntityPowerupSkull::animPowerupSkullIdle[] = {2,1,0,3,1,3,-1};
char EntityPowerupSkull::ENTTYPE = 4;

EntityPowerupSkull::EntityPowerupSkull(void) : Entity(1, 1) { 
	cout << "Warning! EntityPowerupSkull::EntityPowerupSkull() Default Constructor Used" << endl;
	centerX = 8.0f;
	centerY = 8.0f;
	width = 16.0f;
	height = 16.0f;
	setTexture("textures/default/ent/pwrup.png");
	setAnimation(animPowerupSkullIdle);
	mapDataMask = World::MAPDATA_ENTITY;
	entType = ENTTYPE;
}

EntityPowerupSkull::EntityPowerupSkull(float x, float y) : Entity(x, y) { 
	centerX = 8.0f;
	centerY = 8.0f;
	width = 16.0f;
	height = 16.0f;
	setTexture("textures/default/ent/pwrup.png");
	setAnimation(animPowerupSkullIdle);
	mapDataMask = World::MAPDATA_ENTITY;
	entType = ENTTYPE;
}

EntityPowerupSkull::~EntityPowerupSkull(void) {
	//cout << "EntityFlamePower destructor" << endl;
}