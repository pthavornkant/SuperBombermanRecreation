#include "EntityPowerupBlast.h"
#include "World.h"

char EntityPowerupBlast::animPowerupBlastIdle[] = {2,1,0,1,1,1,-1};
char EntityPowerupBlast::ENTTYPE = 1;

EntityPowerupBlast::EntityPowerupBlast(void) : Entity(1, 1) { 
	cout << "Warning! EntityPowerupBlast::EntityPowerupBlast() Default Constructor Used" << endl;
	centerX = 8.0f;
	centerY = 8.0f;
	width = 16.0f;
	height = 16.0f;
	setTexture("textures/default/ent/pwrup.png");
	setAnimation(animPowerupBlastIdle);
	mapDataMask = World::MAPDATA_ENTITY;
	entType = ENTTYPE;
}

EntityPowerupBlast::EntityPowerupBlast(float x, float y) : Entity(x, y) { 
	centerX = 8.0f;
	centerY = 8.0f;
	width = 16.0f;
	height = 16.0f;
	setTexture("textures/default/ent/pwrup.png");
	setAnimation(animPowerupBlastIdle);
	mapDataMask = World::MAPDATA_ENTITY;
	entType = ENTTYPE;
}

EntityPowerupBlast::~EntityPowerupBlast(void) {
	//cout << "EntityFlamePower destructor" << endl;
}