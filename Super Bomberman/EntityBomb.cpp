#include "EntityBomb.h"
#include "World.h"
#include "EntityFire.h"
#include "GameWindow.h"

char EntityBomb::animBombIdle[] = {30,1,0,1,0,0,0,1,1,0,-1};
int EntityBomb::FUSETIME = 150;
int EntityBomb::DEFAULTBLASTRADIUS = 2;

EntityBomb::EntityBomb(void) : Entity(1, 1) {
	cout << "Warning! EntityBomb::EntityBomb() Default Constructor Used" << endl;
	centerX = centerY = 8.0f;
	width = height = 16.0f;
	setTexture("textures/default/ent/bomb.png");
	setAnimation(animBombIdle);
	mapDataMask = World::MAPDATA_BLOCKED | World::MAPDATA_BOMB | World::MAPDATA_UNSAFE | World::MAPDATA_ENTITY;
	fuse = FUSETIME;
	owner = NULL;
	blastRadius = DEFAULTBLASTRADIUS;
}

EntityBomb::EntityBomb(float x, float y) : Entity(x, y) {
	centerX = centerY = 8.0f;
	width = height = 16.0f;
	setTexture("textures/default/ent/bomb.png");
	setAnimation(animBombIdle);
	mapDataMask = World::MAPDATA_BLOCKED | World::MAPDATA_BOMB | World::MAPDATA_UNSAFE | World::MAPDATA_ENTITY;
	fuse = FUSETIME;
	owner = NULL;
	blastRadius = DEFAULTBLASTRADIUS;
}

EntityBomb::EntityBomb(float x, float y, Character* owner, int blastRad) : Entity(x, y) {
	centerX = centerY = 8.0f;
	width = height = 16.0f;
	setTexture("textures/default/ent/bomb.png");
	setAnimation(animBombIdle);
	mapDataMask = World::MAPDATA_BLOCKED | World::MAPDATA_BOMB | World::MAPDATA_UNSAFE | World::MAPDATA_ENTITY;
	fuse = FUSETIME;
	this->owner = owner;
	blastRadius = blastRad;
}

EntityBomb::~EntityBomb(void) {
	//Add to Owner's Bomb Supply
	if(owner) {
		owner->addBomb();
	} else {
		cout << "ERROR: EntityBomb has no owner!" << endl;
	}

	//cout << "Bomb exploding! AKA: EntityBomb::~EntityBomb Destructor called!" << endl;
	GameWindow::world->removeEntity(this);
	GameWindow::world->addEntity(new EntityFire(x, y, EntityFire::FIREDIR_CENTER));
	
	//Creating the Cross Blast
	bool contUp = true, contDown = true, contLeft = true, contRight = true;
	unsigned short* mapData = GameWindow::world->getMapData();
	if(mapData[GameWindow::world->xyToIndex(x + 1, y)] & World::MAPDATA_WORLD) {
		contRight = false;
	}
	if(mapData[GameWindow::world->xyToIndex(x - 1, y)] & World::MAPDATA_WORLD) {
		contLeft = false;
	}
	if(mapData[GameWindow::world->xyToIndex(x, y - 1)] & World::MAPDATA_WORLD) {
		contUp = false;
	}
	if(mapData[GameWindow::world->xyToIndex(x, y + 1)] & World::MAPDATA_WORLD) {
		contDown = false;
	}
	Entity* ent;

	for (int i = 1; i <= blastRadius; i++) {

		//Right Spread
		if(contRight && mapData[GameWindow::world->xyToIndex(x + i, y)] & World::MAPDATA_WORLD) {
			contRight = false;
		} else if(contRight) {
			int dx = x + i, dy = y;
			int index = GameWindow::world->xyToIndex(dx, dy);
			int index2 = GameWindow::world->xyToIndex(dx + 1, dy);

			//Handle Processing Current Space
			if(mapData[index] & World::MAPDATA_ENTITY) {
				ent = GameWindow::world->getEntity(dx, dy);

				//Handle Changing Direction of Currently Existing Fire
				if(mapData[index] & World::MAPDATA_FIRE) {
					EntityFire* fire = (EntityFire*) ent;
					if(fire->direction == EntityFire::FIREDIR_VERT || fire->direction == EntityFire::FIREDIR_TOP || fire->direction == EntityFire::FIREDIR_BOTTOM) {
						fire->setDirection(EntityFire::FIREDIR_CENTER);
					} else if(fire->direction == EntityFire::FIREDIR_LEFT) {
						fire->setDirection(EntityFire::FIREDIR_HORIZ);
					} else if(fire->direction == EntityFire::FIREDIR_RIGHT && i < blastRadius) {
						fire->setDirection(EntityFire::FIREDIR_HORIZ);
					}
				} else {
					if(ent != NULL) {
						ent->destruct();
					}
					contRight = false;
				}
			} else {
				if(i < blastRadius) {
					if(mapData[index2] & World::MAPDATA_WORLD) {
						GameWindow::world->addEntity(new EntityFire(dx, dy, EntityFire::FIREDIR_RIGHT));
						contRight = false;
					} else {
						GameWindow::world->addEntity(new EntityFire(dx, dy, EntityFire::FIREDIR_HORIZ));
					}
				} else {
					GameWindow::world->addEntity(new EntityFire(dx, dy, EntityFire::FIREDIR_RIGHT));
				}
			}
		}

		//Left Spread
		if(contLeft && mapData[GameWindow::world->xyToIndex(x - i, y)] & World::MAPDATA_WORLD) {
			contLeft = false;
		} else if(contLeft) {
			int dx = x - i, dy = y;
			int index = GameWindow::world->xyToIndex(dx, dy);
			int index2 = GameWindow::world->xyToIndex(dx - 1, dy);

			//Handle Processing Current Space
			if(mapData[index] & World::MAPDATA_ENTITY) {
				ent = GameWindow::world->getEntity(dx, dy);
				if(ent != NULL) {
					//Handle Changing Direction of Currently Existing Fire
					if(mapData[index] & World::MAPDATA_FIRE) {
						EntityFire* fire = (EntityFire*) ent;
						if(fire->direction == EntityFire::FIREDIR_VERT || fire->direction == EntityFire::FIREDIR_TOP || fire->direction == EntityFire::FIREDIR_BOTTOM) {
							fire->setDirection(EntityFire::FIREDIR_CENTER);
						} else if(fire->direction == EntityFire::FIREDIR_RIGHT) {
							fire->setDirection(EntityFire::FIREDIR_HORIZ);
						} else if(fire->direction == EntityFire::FIREDIR_LEFT && i < blastRadius) {
							fire->setDirection(EntityFire::FIREDIR_HORIZ);
						}
					} else {
						if(ent != NULL) {
							ent->destruct();
						}
						contLeft = false;
					}
				}
			} else {
				if(i < blastRadius) {
					if(mapData[index2] & World::MAPDATA_WORLD) {
						GameWindow::world->addEntity(new EntityFire(dx, dy, EntityFire::FIREDIR_LEFT));
						contLeft = false;
					} else {
						GameWindow::world->addEntity(new EntityFire(dx, dy, EntityFire::FIREDIR_HORIZ));
					}
				} else {
					GameWindow::world->addEntity(new EntityFire(dx, dy, EntityFire::FIREDIR_LEFT));
				}
			}
		}

		//Up Spread
		if(contUp && mapData[GameWindow::world->xyToIndex(x, y - i)] & World::MAPDATA_WORLD) {
			contUp = false;
		} else if(contUp) {
			int dx = x, dy = y - i;
			int index = GameWindow::world->xyToIndex(dx, dy);
			int index2 = GameWindow::world->xyToIndex(dx, dy - 1);

			//Handle Processing of Current Space
			if(mapData[index] & World::MAPDATA_ENTITY) {
				ent = GameWindow::world->getEntity(dx, dy);

				if(ent != NULL) {
					//Handle Changing Direction of Currently Existing Fire
					if(mapData[index] & World::MAPDATA_FIRE) {
						EntityFire* fire = (EntityFire*) ent;
						if(fire->direction == EntityFire::FIREDIR_HORIZ || fire->direction == EntityFire::FIREDIR_LEFT || fire->direction == EntityFire::FIREDIR_RIGHT) {
							fire->setDirection(EntityFire::FIREDIR_CENTER);
						} else if(fire->direction == EntityFire::FIREDIR_BOTTOM) {
							fire->setDirection(EntityFire::FIREDIR_VERT);
						} else if(fire->direction == EntityFire::FIREDIR_TOP && i < blastRadius) {
							fire->setDirection(EntityFire::FIREDIR_VERT);
						}
					} else {
						if(ent != NULL) {
							ent->destruct();
						}
						contUp = false;
					}
				}
			} else {
				if(i < blastRadius) {
					if(mapData[index2] & World::MAPDATA_WORLD) {
						GameWindow::world->addEntity(new EntityFire(dx, dy, EntityFire::FIREDIR_TOP));
						contUp = false;
					} else {
						GameWindow::world->addEntity(new EntityFire(dx, dy, EntityFire::FIREDIR_VERT));
					}
				} else {
					GameWindow::world->addEntity(new EntityFire(dx, dy, EntityFire::FIREDIR_TOP));
				}
			}
		}

		//Down Spread
		if(contDown && mapData[GameWindow::world->xyToIndex(x, y + i)] & World::MAPDATA_WORLD) {
			contDown = false;
		} else if(contDown) {
			int dx = x, dy = y + i;
			int index = GameWindow::world->xyToIndex(dx, dy);
			int index2 = GameWindow::world->xyToIndex(dx, dy + 1);

			//Handle Processing of Current Space
			if(mapData[index] & World::MAPDATA_ENTITY) {
				ent = GameWindow::world->getEntity(dx, dy);

				//Handle Changing Direction of Currently Existing Fire
				if(mapData[index] & World::MAPDATA_FIRE) {
					EntityFire* fire = (EntityFire*) ent;
					if(fire->direction == EntityFire::FIREDIR_HORIZ || fire->direction == EntityFire::FIREDIR_LEFT || fire->direction == EntityFire::FIREDIR_RIGHT) {
						fire->setDirection(EntityFire::FIREDIR_CENTER);
					} else if(fire->direction == EntityFire::FIREDIR_TOP) {
						fire->setDirection(EntityFire::FIREDIR_VERT);
					} else if(fire->direction == EntityFire::FIREDIR_BOTTOM && i < blastRadius) {
						fire->setDirection(EntityFire::FIREDIR_VERT);
					}
				} else {
					if(ent != NULL) {
						ent->destruct();
					}
					contDown = false;
				}
			} else {
				if(i < blastRadius) {
					if(mapData[index2] & World::MAPDATA_WORLD) {
						GameWindow::world->addEntity(new EntityFire(dx, dy, EntityFire::FIREDIR_BOTTOM));
						contDown = false;
					} else {
						GameWindow::world->addEntity(new EntityFire(dx, dy, EntityFire::FIREDIR_VERT));
					}
				} else {
					GameWindow::world->addEntity(new EntityFire(dx, dy, EntityFire::FIREDIR_BOTTOM));
				}
			}
		}

		/*Notes: Currently the cross shape pattern amusingly enough, does remove blocks, but not properly.
		It replaces the block entity completely then deletes itself (which is kind of amusing but not
		how we want this to be done). 

		I chose 2 for now because 3 is actually pretty big considering how small our actual map is.
		
		Things to add:
		+Check for Collision (I don't know how to check mapData for Collision Data)
		+Set directions for newly created fire entities (Not sure how to pass in directions either.)*/
	}
}

void EntityBomb::update() {
	incrAnim();
	fuse--;

	if (fuse == 0)
	{
		this->~EntityBomb();
	}
}

void EntityBomb::destruct() {
	this->~EntityBomb();
}

int EntityBomb::getBlastRadius() {
	return blastRadius;
}