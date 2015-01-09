#include "EntitydBlock.h"
#include "World.h"
#include "EntityPowerupBlast.h"
#include "EntityPowerupBomb.h"
#include "EntityPowerupSpeed.h"
#include "GameWindow.h"

char EntitydBlock::animDestIdle[] = {60,0,0,0,-1};
char EntitydBlock::animDestBreak[] = {8,0,0,1,0,2,0,3,1,0,1,1,-1};

EntitydBlock::EntitydBlock(void) : Entity(1, 1) {
	cout << "Warning! EntitydBlock::EntitydBlock() Default Constructor Used" << endl;
	centerX = centerY = 8.0f;
	width = height = 16.0f;
	setTexture("textures/default/ent/destructible.png");
	setAnimation(animDestIdle);
	mapDataMask = World::MAPDATA_BLOCKED | World::MAPDATA_DESTRUCTIBLE | World::MAPDATA_ENTITY;
}

EntitydBlock::EntitydBlock(float x, float y) : Entity(x, y) {
	centerX = centerY = 8.0f;
	width = height = 16.0f;
	setTexture("textures/default/ent/destructible.png");
	setAnimation(animDestIdle);
	mapDataMask = World::MAPDATA_BLOCKED | World::MAPDATA_DESTRUCTIBLE | World::MAPDATA_ENTITY;
}

EntitydBlock::~EntitydBlock(void) {
	int decision = rand() % 100;
	if(decision < 70) {
		//No Powerup
	} else if(decision < 80) {
		GameWindow::world->addEntity(new EntityPowerupBlast(x, y));
	} else if(decision < 90) {
		GameWindow::world->addEntity(new EntityPowerupBomb(x, y));
	} else {
		GameWindow::world->addEntity(new EntityPowerupSpeed(x, y));
	}
}

void EntitydBlock::update() {
	incrAnim();
}

void EntitydBlock::destruct() {
	if(!destructing) {
		setAnimation(animDestBreak);
		animFunc = animDestructFunc;
		destructing = true;
	}
}