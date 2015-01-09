#include "EntityFire.h"
#include "World.h"

char EntityFire::animFireCenterIdle[] = {4,0,0,2,1,2,2,2,3,2,4,2,3,2,2,2,1,2,0,2,-1};
char EntityFire::animFireHorizIdle[] = {4,0,0,1,1,1,2,1,3,1,4,1,3,1,2,1,1,1,0,1,-1};
char EntityFire::animFireVertIdle[] = {4,0,0,3,1,3,2,3,3,3,4,3,3,3,2,3,1,3,0,3,-1};
char EntityFire::animFireTopIdle[] = {4,0,0,4,1,4,2,4,3,4,4,4,3,4,2,4,1,4,0,4,-1};
char EntityFire::animFireBottomIdle[] = {4,0,0,6,1,6,2,6,3,6,4,6,3,6,2,6,1,6,0,6,-1};
char EntityFire::animFireLeftIdle[] = {4,0,0,0,1,0,2,0,3,0,4,0,3,0,2,0,1,0,0,0,-1};
char EntityFire::animFireRightIdle[] = {4,0,0,5,1,5,2,5,3,5,4,5,3,5,2,5,1,5,0,5,-1};

EntityFire::EntityFire(void) : Entity(1, 1) { 
	cout << "Warning! EntityFire::EntityFire() Default Constructor Used" << endl;
	centerX = 8.0f;
	centerY = 8.0f;
	width = 16.0f;
	height = 16.0f;
	setTexture("textures/default/ent/fire.png");
	mapDataMask = World::MAPDATA_KILL | World::MAPDATA_UNSAFE | World::MAPDATA_ENTITY | World::MAPDATA_FIRE;
	animFunc = animDestructFunc;
	setDirection(FIREDIR_CENTER);
}

EntityFire::EntityFire(float x, float y) : Entity(x, y) { 
	centerX = 8.0f;
	centerY = 8.0f;
	width = 16.0f;
	height = 16.0f;
	setTexture("textures/default/ent/fire.png");
	mapDataMask = World::MAPDATA_KILL | World::MAPDATA_UNSAFE | World::MAPDATA_ENTITY | World::MAPDATA_FIRE;
	animFunc = animDestructFunc;
	setDirection(FIREDIR_CENTER);
}

EntityFire::EntityFire(float x, float y, char dir) : Entity(x, y) { 
	centerX = 8.0f;
	centerY = 8.0f;
	width = 16.0f;
	height = 16.0f;
	setTexture("textures/default/ent/fire.png");
	mapDataMask = World::MAPDATA_KILL | World::MAPDATA_UNSAFE | World::MAPDATA_ENTITY | World::MAPDATA_FIRE;
	animFunc = animDestructFunc;
	setDirection(dir);
}

EntityFire::~EntityFire(void) {
	//cout << "Flames dissipating! AKA: EntityFire::~EntityFire Destructor called!" << endl;
}

void EntityFire::update() {
	incrAnim();
}

void EntityFire::setDirection(char dir) {
	direction = dir;
	switch(dir) {
	case FIREDIR_CENTER:
		setAnimation(animFireCenterIdle);
		break;
	case FIREDIR_HORIZ:
		setAnimation(animFireHorizIdle);
		break;
	case FIREDIR_VERT:
		setAnimation(animFireVertIdle);
		break;
	case FIREDIR_TOP:
		setAnimation(animFireTopIdle);
		break;
	case FIREDIR_BOTTOM:
		setAnimation(animFireBottomIdle);
		break;
	case FIREDIR_LEFT:
		setAnimation(animFireLeftIdle);
		break;
	case FIREDIR_RIGHT:
		setAnimation(animFireRightIdle);
		break;
	default:
		setAnimation(animFireCenterIdle);
		break;
	}
}