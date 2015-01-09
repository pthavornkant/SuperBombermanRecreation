#include "Character.h"
#include "GameWindow.h"
#include "World.h"
#include "EntityBomb.h"
#include "EntityPowerupBlast.h"
#include "EntityPowerupBomb.h"
#include "EntityPowerupSpeed.h"
#include "EntityPowerupSkull.h"
#include "EntitydBlock.h"

#include <iostream>
using namespace std;

//Animation {Delay, Repeat, AnimRow0, AnimCol0, AnimRow1, AnimCol1, ...};
char Character::animIdleUp[] = {60,0,3,0,-1};
char Character::animIdleDown[] = {60,0,0,0,-1};
char Character::animIdleLeft[] = {60,0,1,0,-1};
char Character::animIdleRight[] = {60,0,2,0,-1};
char Character::animMoveUp[] = {12,1,3,2,3,0,3,1,3,0,-1};
char Character::animMoveDown[] = {12,1,0,1,0,0,0,2,0,0,-1};
char Character::animMoveLeft[] = {12,1,1,1,1,0,1,2,1,0,-1};
char Character::animMoveRight[] = {12,1,2,2,2,0,2,1,2,0,-1};
char Character::animDeath[] = {12,0,4,0,4,1,4,2,4,3,4,4,4,5,-1};

int Character::DEFAULTBOMBSUPPLY = 1;
int Character::DEFAULTBOMBBLAST = 2;
float Character::DEFAULTSPEED = 0.08f;
char Character::PWRUP_BOMB_MAX = 4;
char Character::PWRUP_BLAST_MAX = 4;
char Character::PWRUP_SPEED_MAX = 4;

Character::Character(void) : Entity(0, 0) {
	cout << "Warning! Character::Character() Default Constructor Used" << endl;
	keyForward = keyBackward = keyLeft = keyRight = keyBomb = keyBlock = false;
	setAnimation(animIdleDown);
	prevDirection = DOWN;
	prevMovement = false;
	prevSwitch = 0;
	dead = false;
	bombSupply = DEFAULTBOMBSUPPLY;
	bombBlast = DEFAULTBOMBBLAST;
	speed = DEFAULTSPEED;
	pwrup_bomb = pwrup_blast = pwrup_speed = 0;
}

Character::Character(float x, float y) : Entity(x, y) {
	keyForward = keyBackward = keyLeft = keyRight = keyBomb = keyBlock = false;
	setAnimation(animIdleDown);
	prevDirection = DOWN;
	prevMovement = false;
	prevSwitch = 0;
	dead = false;
	bombSupply = DEFAULTBOMBSUPPLY;
	bombBlast = DEFAULTBOMBBLAST;
	speed = DEFAULTSPEED;
	pwrup_bomb = pwrup_blast = pwrup_speed = 0;
}

Character::~Character(void) {
}

/*
void Character::drawShift(float shift) {
	if(visible) {
		char animX, animY;
		float x = (this->x * 16.0f) + 8.0f - centerX;
		float y = (this->y * 16.0f) + 8.0f - centerY;
		getAnimationCoords(animX, animY);
		glBindTexture(GL_TEXTURE_2D, this->texture);
		glBegin(GL_QUADS);
		glTexCoord2f(animX * dTW, animY * dTH);
		glVertex3f(x, y, this->y - 13.0f - shift);
		glTexCoord2f(animX * dTW + dTW, animY * dTH);
		glVertex3f(x + width, y, this->y - 13.0f - shift);
		glTexCoord2f(animX * dTW + dTW, animY * dTH + dTH);
		glVertex3f(x + width, y + height, this->y - 13.0f - shift);
		glTexCoord2f(animX * dTW, animY * dTH + dTH);
		glVertex3f(x, y + height, this->y - 13.0f - shift);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Character::draw() {
	char animX, animY;
	float x = (this->x * 16.0f) + 8.0f - centerX;
	float y = (this->y * 16.0f) + 8.0f - centerY;
	getAnimationCoords(animX, animY);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	glBegin(GL_QUADS);
	glTexCoord2f(animX * dTW, animY * dTH);
	glVertex3f(x, y, this->y - 13.0f);
	glTexCoord2f(animX * dTW + dTW, animY * dTH);
	glVertex3f(x + width, y, this->y - 13.0f);
	glTexCoord2f(animX * dTW + dTW, animY * dTH + dTH);
	glVertex3f(x + width, y + height, this->y - 13.0f);
	glTexCoord2f(animX * dTW, animY * dTH + dTH);
	glVertex3f(x, y + height, this->y - 13.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
}
*/

void Character::update() {
	incrAnim();

	if(!dead) {
		World* world = GameWindow::world;
		unsigned char* blockData = world->getBlockData();
		Block* blockList = getBlockList();

		//Varables for Previous Movement Data
		prevDirection;
		prevMovement;

		//Store Previous Movement Status before Changing
		if(vx > 0.01f || vx < -0.01f || vy > 0.01f || vy < -0.01f) {
			prevMovement = true;
		} else {
			prevMovement = false;
		}

		/*I think we could implement one of the power ups to include a movement speed boost. 
		There's a rollerskate icon in the textures, you can  use that for speed boosts.
		Checking if you have a power up seems to be like all you'd have to add, so just a few 
		if statements in the section below. I think a good speed bonus rate would be 0.12*/

		//Handle Up/Down Movement
		if(keyForward) {
			/*PSEUDO CODE
			if (speedboost) {
				vy = -0.12f;
			} else if etc...*/
			vy = -speed;
		} else if(keyBackward) {
			vy = speed;
		}
		if(keyForward && keyBackward || !keyForward && !keyBackward) {
			vy = 0.0f;
		}

		//Handle Left/Right Movement
		if(keyLeft) {
			vx = -speed;
		} else if(keyRight) {
			vx = speed;
		}
		if(keyLeft && keyRight || !keyLeft && !keyRight) {
			vx = 0.0f;
		}

		//Current Position Data (Rounded)
		unsigned int cx = (unsigned int)(x + 0.5f);
		unsigned int cy = (unsigned int)(y + 0.5f);

		//Preliminary Bomb Limit Code (Should it be done in EntityBomb.cpp instead?)
		//int bomblimit = 3;
		//static int cbombs = 0;

		//Create Bomb at Current Position
		if(keyBomb){
			unsigned int index = GameWindow::world->xyToIndex(cx, cy);
			Entity** ents = world->getEntities();
			if(ents[index] == NULL && bombSupply > 0) {
				world->addEntity(new EntityBomb(cx, cy, this, bombBlast));
				bombSupply--;
				//cbombs++;
				//cout << "Current Bombs: " << cbombs << endl;
			}
		}

		//Create Destructible Block at Current Position
		/*Maybe make this a power up in actual game?
	
		Would just have to put this all in an if statement, something like
		if (hasBlock) then do the rest, otherwise don't allow this to function?*/
		if(keyBlock){
			unsigned int index = GameWindow::world->xyToIndex(cx,cy);
			Entity** ents = world->getEntities();
			if(ents[index] == NULL) {
				world->addEntity(new EntitydBlock(cx, cy));
				//ents[index] = new EntitydBlock(cx, cy);
			}
		}

		/*
		//Prevent Character from Trying to walk into Areas that are occupied
		if((x >= cx && vx > 0.0f && blockList[blockData[world->xyToIndex(cx + 1, cy)]].collision) ||
			(x <= cx && vx < 0.0f && blockList[blockData[world->xyToIndex(cx - 1, cy)]].collision)) {
			x = cx;
			vx = 0.0f;
		}
		if((y >= cy && vy > 0.0f && blockList[blockData[world->xyToIndex(cx, cy + 1)]].collision == true) ||
			(y <= cy && vy < 0.0f && blockList[blockData[world->xyToIndex(cx, cy - 1)]].collision)) {
			y = cy;
			vy = 0.0f;
		}
		*/
		//Prevent Character from Trying to walk into Areas that are occupied
		if((x >= cx && vx > 0.0f && (GameWindow::world->getMapData())[GameWindow::world->xyToIndex(cx + 1, cy)] & World::MAPDATA_BLOCKED) ||
			(x <= cx && vx < 0.0f && (GameWindow::world->getMapData())[GameWindow::world->xyToIndex(cx - 1, cy)] & World::MAPDATA_BLOCKED)) {
			x = cx;
			vx = 0.0f;
		}
		if((y >= cy && vy > 0.0f && (GameWindow::world->getMapData())[GameWindow::world->xyToIndex(cx, cy + 1)] & World::MAPDATA_BLOCKED) ||
			(y <= cy && vy < 0.0f && (GameWindow::world->getMapData())[GameWindow::world->xyToIndex(cx, cy - 1)] & World::MAPDATA_BLOCKED)) {
			y = cy;
			vy = 0.0f;
		}

		//World Collision Prevention
		prevSwitch;
		float dx = (x - cx);
		float dy = (y - cy);

		//Prevent Position Correction After Direction Switch
		if(prevSwitch <= 0) {

			//Position Correction for Intersections
			if(prevDirection == UP || prevDirection == DOWN) {
				if(vx > 0.0f || vx < 0.0f) {
					if(dy < -0.1f) {
						vy = 0.08f;
						vx = 0.0f;
					} else if(dy > 0.1f) {
						vy = -0.08f;
						vx = 0.0f;
					} else {
						prevSwitch = 15;
						vy = 0;
						y = cy;
					}
				}
			} else {
				if(vy > 0.0f || vy < 0.0f) {
					if(dx < -0.1f) {
						vx = 0.08f;
						vy = 0.0f;
					} else  if(dx > 0.1f) {
						vx = -0.08f;
						vy = 0.0f;
					} else {
						prevSwitch = 15;
						vx = 0;
						x = cx;
					}
				}
			}
		} else {
			//Decrement Position Correction Counter
			prevSwitch--;

			//Intersection Rapid Direction Change Prevention
			if(prevDirection == LEFT || prevDirection == RIGHT) {
				if(vy >= 0.01f || vy <= -0.01f) {
					vy = 0.0f;
				}
			} else {
				if(vx >= 0.01f || vx <= -0.01f) {
					vx = 0.0f;
				}
			}
		}

		//Is the Character Currently Moving?
		bool currentMovement = (vx >= 0.01f || vx <= -0.01f || vy >= 0.01f || vy <= -0.01f);

		//Animation Change Detection
		if(prevDirection == DOWN) {
			if(vx > 0.0f) {
				setAnimation(animMoveRight);
				prevDirection = RIGHT;
			} else if(vx < 0.0f) {
				setAnimation(animMoveLeft);
				prevDirection = LEFT;
			} else if(vy < 0.0f) {
				setAnimation(animMoveUp);
				prevDirection = UP;
			} else {
				if(prevMovement && !currentMovement) {
					setAnimation(animIdleDown);
				} else if(!prevMovement && currentMovement) {
					setAnimation(animMoveDown);
				}
			}
		} else if(prevDirection == UP) {
			if(vx > 0.0f) {
				setAnimation(animMoveRight);
				prevDirection = RIGHT;
			} else if(vx < 0.0f) {
				setAnimation(animMoveLeft);
				prevDirection = LEFT;
			} else if(vy > 0.0f) {
				setAnimation(animMoveDown);
				prevDirection = DOWN;
			} else {
				if(prevMovement && !currentMovement) {
					setAnimation(animIdleUp);
				} else if(!prevMovement && currentMovement) {
					setAnimation(animMoveUp);
				}
			}
		} else if(prevDirection == LEFT) {
			if(vx > 0.0f) {
				setAnimation(animMoveRight);
				prevDirection = RIGHT;
			} else if(vy > 0.0f) {
				setAnimation(animMoveDown);
				prevDirection = DOWN;
			} else if(vy < 0.0f) {
				setAnimation(animMoveUp);
				prevDirection = UP;
			} else {
				if(prevMovement && !currentMovement) {
					setAnimation(animIdleLeft);
				} else if(!prevMovement && currentMovement) {
					setAnimation(animMoveLeft);
				}
			}
		} else if(prevDirection == RIGHT) {
			if(vx < 0.0f) {
				setAnimation(animMoveLeft);
				prevDirection = LEFT;
			} else if(vy > 0.0f) {
				setAnimation(animMoveDown);
				prevDirection = DOWN;
			} else if(vy < 0.0f) {
				setAnimation(animMoveUp);
				prevDirection = UP;
			} else {
				if(prevMovement && !currentMovement) {
					setAnimation(animIdleRight);
				} else if(!prevMovement && currentMovement) {
					setAnimation(animMoveRight);
				}
			}
		}

		x += vx;
		y += vy;

		//Post-Movement Checks (Death, Powerups, etc.)
		cx = (int)(x + 0.5f);
		cy = (int)(y + 0.5f);
		unsigned short* mapData = GameWindow::world->getMapData();
		
		if(mapData[GameWindow::world->xyToIndex(cx, cy)] & World::MAPDATA_KILL) {
			dead = true;
			setAnimation(animDeath);
			animFunc = animHideFunc;
		}

		if(mapData[GameWindow::world->xyToIndex(cx, cy)] & World::MAPDATA_ENTITY) {
			Entity* ent = world->getEntity(cx,cy);
			char type = ent->getType();
			if(type == EntityPowerupBlast::ENTTYPE) {
				pwrup_blast++;
				if(pwrup_blast > PWRUP_BLAST_MAX) {
					pwrup_blast = PWRUP_BLAST_MAX;
				} else {
					bombBlast++;
				}
				ent->~Entity();
			} else if(type == EntityPowerupBomb::ENTTYPE) {
				pwrup_bomb++;
				if(pwrup_bomb > PWRUP_BOMB_MAX) {
					pwrup_bomb = PWRUP_BOMB_MAX;
				} else {
					bombSupply++;
				}
				ent->~Entity();
			} else if(type == EntityPowerupSkull::ENTTYPE) {
				//IDK
				ent->~Entity();
			} else if(type == EntityPowerupSpeed::ENTTYPE) {
				pwrup_speed++;
				if(pwrup_speed > PWRUP_SPEED_MAX) {
					pwrup_speed = PWRUP_SPEED_MAX;
				} else {
					speed += 0.01;
				}
				ent->~Entity();
			}
		}
	}
}

void Character::addBomb() {
	bombSupply++;
}

bool Character::isAlive() {
	return !dead;
}