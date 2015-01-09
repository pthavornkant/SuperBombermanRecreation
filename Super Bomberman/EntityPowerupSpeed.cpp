#include "EntityPowerupSpeed.h"
#include "World.h"

char EntityPowerupSpeed::animPowerupSpeedIdle[] = {2,1,0,2,1,2,-1};
char EntityPowerupSpeed::ENTTYPE = 3;

EntityPowerupSpeed::EntityPowerupSpeed(void) : Entity(1, 1) { 
	cout << "Warning! EntityPowerupSpeed::EntityPowerupSpeed() Default Constructor Used" << endl;
	centerX = 8.0f;
	centerY = 8.0f;
	width = 16.0f;
	height = 16.0f;
	setTexture("textures/default/ent/pwrup.png");
	setAnimation(animPowerupSpeedIdle);
	mapDataMask = World::MAPDATA_ENTITY;
	entType = ENTTYPE;
}

EntityPowerupSpeed::EntityPowerupSpeed(float x, float y) : Entity(x, y) { 
	centerX = 8.0f;
	centerY = 8.0f;
	width = 16.0f;
	height = 16.0f;
	setTexture("textures/default/ent/pwrup.png");
	setAnimation(animPowerupSpeedIdle);
	mapDataMask = World::MAPDATA_ENTITY;
	entType = ENTTYPE;
}

EntityPowerupSpeed::~EntityPowerupSpeed(void) {
	//cout << "EntityFlamePower destructor" << endl;
}