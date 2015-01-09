#include "World.h"

#include <iostream>
#include <Windows.h>
#include <mmsystem.h>
#include <time.h>
#include "EntitydBlock.h"
#include "EntityBomb.h"
#include <typeinfo>
using namespace std;

World::World(void) {
	cout << "Warning! World::World() Default Constructor Used" << endl;
	GLOBWORLD = this;
	_x = 15;
	_y = 13;
	endTriggered = false;
	timer = 120 * 60 + 59;
	ents = new Entity*[_x * _y];
	for(unsigned int i = 0; i < _x * _y; i++) {
		ents[i] = NULL;
	}
	for(unsigned int i = 0; i < MAXPLAYERS; i++) {
		players[i] = NULL;
	}
	blockData = new unsigned char[_x * _y];
	mapData = new unsigned short[_x * _y];
	generateBlockList();
	generateTerrain();
}

World::World(int x, int y) {
	GLOBWORLD = this;
	unsigned int i, j;
	_x = x; _y = y;
	endTriggered = false;
	timer = 120 * 60 + 59;
	ents = new Entity*[_x * _y];
	for(unsigned int i = 0; i < _x * _y; i++) {
		ents[i] = NULL;
	}
	for(unsigned int i = 0; i < MAXPLAYERS; i++) {
		players[i] = NULL;
	}
	blockData = new unsigned char[_x * _y];
	mapData = new unsigned short[_x * _y];
	generateBlockList();
	generateTerrain();
}

World::~World(void) {
	delete[] players;
	delete blockData;
	delete mapData;
}

Player* World::getPlayer(int num) {
	return players[num - 1];
}

Entity** World::getEntities() {
	return ents;
}

void World::setBlock(unsigned char type, unsigned int& x, unsigned int& y) {
	blockData[xyToIndex(x, y)] = type;
}

unsigned char World::getBlock(unsigned int& x, unsigned int& y) {
	return blockData[xyToIndex(x, y)];
}

void World::generateTerrain() {
	srand(time(NULL));
	for(unsigned j = 0; j < _y; j++) {
		for(unsigned i = 0; i < _x; i++) {
			if(j == 0 || j == _y - 1 || i == 0 || i == _x - 1) {
				blockData[xyToIndex(i, j)] = BLOCKTYPE_WALL;
			} else if(i % 2 == 0 && j % 2 == 0) {
				blockData[xyToIndex(i, j)] = BLOCKTYPE_WALL;
			} else {
				blockData[xyToIndex(i, j)] = BLOCKTYPE_BLANK;
			}

			//Destructibles
			if(i > 0 && j > 0 && i < _x - 1 && j < _y - 1) {
				if(!((i < 3 && j < 3) || (i > (_x - 4) && j < 3) || (i > (_x - 4) && j > (_y - 4)) || (i < 3 && j > (_y - 4)))) {
					if((i % 2 == 1 || j % 2 == 1)) {
						if((rand() % 100) < 80) {
							addEntity(new EntitydBlock(i, j));
						}
					}
				}
			}
		}
	}
}

void World::drawHUD() {
	float width, height;
	static GLuint hudset = TextureHandler::loadTexture("textures/default/hud/hudset.png", width, height);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);

	//Draw Header BG
	glColor3ub(240, 128, 0);
	glVertex3f(0.0f, 0.0f, -100.0f);
	glVertex3f(240.0f, 0.0f, -100.0f);
	glVertex3f(240.0f, 32.0f, -100.0f);
	glVertex3f(0.0f, 32.0f, -100.0f);
	glColor3ub(144, 16, 0);
	glVertex3f(0.0f, 31.0f, -99.0f);
	glVertex3f(240.0f, 31.0f, -99.0f);
	glVertex3f(240.0f, 32.0f, -99.0f);
	glVertex3f(0.0f, 32.0f, -99.0f);

	glEnd();

	glColor3ub(255, 255, 255);
	glBindTexture(GL_TEXTURE_2D, hudset);

	//Offset
	static float oy = 24.0f / height;
	static float dtx = 16.0f / width;
	static float dty = 16.0f / height;

	//Score Texture Offset
	float dnumx, dnumy;

	//Test For Score Displaying
	static int counter = 0;
	int timeLeft = timer / 60;
	int score = (counter++ / 60) % 10;

	//WHITE BOMBERMAN
	if(players[0] != NULL) {
		glPushMatrix();

		bool dead = !players[0]->isAlive();
		//Head
		glTranslatef(16.0f, 8.0f, 0.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, oy + dead * dty);
		glVertex3f(0.0f, 0.0f, -50.0f);
		glTexCoord2f(dtx, oy + dead * dty);
		glVertex3f(16.0f, 0.0f, -50.0f);
		glTexCoord2f(dtx, oy + dty + dead * dty);
		glVertex3f(16.0f, 16.0f, -50.0f);
		glTexCoord2f(0.0f, oy + dty + dead * dty);
		glVertex3f(0.0f, 16.0f, -50.0f);

		//Score
		xyFromNum(players[0]->score, dnumx, dnumy);
		glTexCoord2f(dnumx, dnumy);
		glVertex3f(16.0f, 2.0f, -50.0f);
		glTexCoord2f(dnumx + (8.0f/64.0f), dnumy);
		glVertex3f(24.0f, 2.0f, -50.0f);
		glTexCoord2f(dnumx + (8.0f/64.0f), dnumy + (12.0f/64.0f));
		glVertex3f(24.0f, 14.0f, -50.0f);
		glTexCoord2f(dnumx, dnumy + (12.0f/64.0f));
		glVertex3f(16.0f, 14.0f, -50.0f);

		glEnd();
		glPopMatrix();
	}

	//BLACK BOMBERMAN
	if(players[1] != NULL) {
		glPushMatrix();

		bool dead = !players[1]->isAlive();
		//Head
		glTranslatef(64.0f, 8.0f, 0.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(dtx, oy + dead * dty);
		glVertex3f(0.0f, 0.0f, -50.0f);
		glTexCoord2f(2 * dtx, oy + dead * dty);
		glVertex3f(16.0f, 0.0f, -50.0f);
		glTexCoord2f(2 * dtx, oy + dty + dead * dty);
		glVertex3f(16.0f, 16.0f, -50.0f);
		glTexCoord2f(dtx, oy + dty + dead * dty);
		glVertex3f(0.0f, 16.0f, -50.0f);

		//Score
		xyFromNum(players[1]->score, dnumx, dnumy);
		glTexCoord2f(dnumx, dnumy);
		glVertex3f(16.0f, 2.0f, -50.0f);
		glTexCoord2f(dnumx + (8.0f/64.0f), dnumy);
		glVertex3f(24.0f, 2.0f, -50.0f);
		glTexCoord2f(dnumx + (8.0f/64.0f), dnumy + (12.0f/64.0f));
		glVertex3f(24.0f, 14.0f, -50.0f);
		glTexCoord2f(dnumx, dnumy + (12.0f/64.0f));
		glVertex3f(16.0f, 14.0f, -50.0f);

		glEnd();
		glPopMatrix();
	}

	//BLUE BOMBERMAN
	if(players[2] != NULL) {
		glPushMatrix();

		bool dead = !players[2]->isAlive();
		//Head
		glTranslatef(152.0f, 8.0f, 0.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(2 * dtx, oy + dead * dty);
		glVertex3f(0.0f, 0.0f, -50.0f);
		glTexCoord2f(3 * dtx, oy + dead * dty);
		glVertex3f(16.0f, 0.0f, -50.0f);
		glTexCoord2f(3 * dtx, oy + dty + dead * dty);
		glVertex3f(16.0f, 16.0f, -50.0f);
		glTexCoord2f(2 * dtx, oy + dty + dead * dty);
		glVertex3f(0.0f, 16.0f, -50.0f);

		//Score
		xyFromNum(players[2]->score, dnumx, dnumy);
		glTexCoord2f(dnumx, dnumy);
		glVertex3f(16.0f, 2.0f, -50.0f);
		glTexCoord2f(dnumx + (8.0f/64.0f), dnumy);
		glVertex3f(24.0f, 2.0f, -50.0f);
		glTexCoord2f(dnumx + (8.0f/64.0f), dnumy + (12.0f/64.0f));
		glVertex3f(24.0f, 14.0f, -50.0f);
		glTexCoord2f(dnumx, dnumy + (12.0f/64.0f));
		glVertex3f(16.0f, 14.0f, -50.0f);

		glEnd();
		glPopMatrix();
	}

	//RED BOMBERMAN
	if(players[3] != NULL) {
		glPushMatrix();

		bool dead = !players[3]->isAlive();
		//Head
		glTranslatef(200.0f, 8.0f, 0.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(3 * dtx, oy + dead * dty);
		glVertex3f(0.0f, 0.0f, -50.0f);
		glTexCoord2f(4 * dtx, oy + dead * dty);
		glVertex3f(16.0f, 0.0f, -50.0f);
		glTexCoord2f(4 * dtx, oy + dty + dead * dty);
		glVertex3f(16.0f, 16.0f, -50.0f);
		glTexCoord2f(3 * dtx, oy + dty + dead * dty);
		glVertex3f(0.0f, 16.0f, -50.0f);

		//Score
		xyFromNum(players[3]->score, dnumx, dnumy);
		glTexCoord2f(dnumx, dnumy);
		glVertex3f(16.0f, 2.0f, -50.0f);
		glTexCoord2f(dnumx + (8.0f/64.0f), dnumy);
		glVertex3f(24.0f, 2.0f, -50.0f);
		glTexCoord2f(dnumx + (8.0f/64.0f), dnumy + (12.0f/64.0f));
		glVertex3f(24.0f, 14.0f, -50.0f);
		glTexCoord2f(dnumx, dnumy + (12.0f/64.0f));
		glVertex3f(16.0f, 14.0f, -50.0f);

		glEnd();
		glPopMatrix();
	}

	//TIMER
	glPushMatrix();
	glTranslatef(104.0f, 10.0f, 0.0f);
	glBegin(GL_QUADS);
	//Minutes
	xyFromNum(timeLeft / 60, dnumx, dnumy);
	glTexCoord2f(dnumx, dnumy);
	glVertex3f(0.0f, 0.0f, -50.0f);
	glTexCoord2f(dnumx + (8.0f/64.0f), dnumy);
	glVertex3f(8.0f, 0.0f, -50.0f);
	glTexCoord2f(dnumx + (8.0f/64.0f), dnumy + (12.0f/64.0f));
	glVertex3f(8.0f, 12.0f, -50.0f);
	glTexCoord2f(dnumx, dnumy + (12.0f/64.0f));
	glVertex3f(0.0f, 12.0f, -50.0f);
	//Colon
	dnumx = (16.0f / 64.0f); dnumy = (12.0f / 64.0f);
	glTexCoord2f(dnumx, dnumy);
	glVertex3f(8.0f, 0.0f, -50.0f);
	glTexCoord2f(dnumx + (8.0f/64.0f), dnumy);
	glVertex3f(16.0f, 0.0f, -50.0f);
	glTexCoord2f(dnumx + (8.0f/64.0f), dnumy + (12.0f/64.0f));
	glVertex3f(16.0f, 12.0f, -50.0f);
	glTexCoord2f(dnumx, dnumy + (12.0f/64.0f));
	glVertex3f(8.0f, 12.0f, -50.0f);
	//Tens (of Seconds)
	xyFromNum((timeLeft % 60) / 10, dnumx, dnumy);
	glTexCoord2f(dnumx, dnumy);
	glVertex3f(16.0f, 0.0f, -50.0f);
	glTexCoord2f(dnumx + (8.0f/64.0f), dnumy);
	glVertex3f(24.0f, 0.0f, -50.0f);
	glTexCoord2f(dnumx + (8.0f/64.0f), dnumy + (12.0f/64.0f));
	glVertex3f(24.0f, 12.0f, -50.0f);
	glTexCoord2f(dnumx, dnumy + (12.0f/64.0f));
	glVertex3f(16.0f, 12.0f, -50.0f);
	//Seconds
	xyFromNum(timeLeft % 10, dnumx, dnumy);
	glTexCoord2f(dnumx, dnumy);
	glVertex3f(24.0f, 0.0f, -50.0f);
	glTexCoord2f(dnumx + (8.0f/64.0f), dnumy);
	glVertex3f(32.0f, 0.0f, -50.0f);
	glTexCoord2f(dnumx + (8.0f/64.0f), dnumy + (12.0f/64.0f));
	glVertex3f(32.0f, 12.0f, -50.0f);
	glTexCoord2f(dnumx, dnumy + (12.0f/64.0f));
	glVertex3f(24.0f, 12.0f, -50.0f);
	glEnd();
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);

	glBegin(GL_QUADS);
	glColor3ub(144, 16, 0);
	if(players[0] != NULL) {
		glVertex3f(32.0f, 9.0f, -60.0f);
		glVertex3f(40.0f, 9.0f, -60.0f);
		glVertex3f(40.0f, 23.0f, -60.0f);
		glVertex3f(32.0f, 23.0f, -60.0f);
	}
	if(players[1] != NULL) {
		glVertex3f(80.0f, 9.0f, -60.0f);
		glVertex3f(88.0f, 9.0f, -60.0f);
		glVertex3f(88.0f, 23.0f, -60.0f);
		glVertex3f(80.0f, 23.0f, -60.0f);
	}
	glVertex3f(104.0f, 9.0f, -60.0f);
	glVertex3f(136.0f, 9.0f, -60.0f);
	glVertex3f(136.0f, 23.0f, -60.0f);
	glVertex3f(104.0f, 23.0f, -60.0f);
	if(players[2] != NULL) {
		glVertex3f(168.0f, 9.0f, -60.0f);
		glVertex3f(176.0f, 9.0f, -60.0f);
		glVertex3f(176.0f, 23.0f, -60.0f);
		glVertex3f(168.0f, 23.0f, -60.0f);
	}
	if(players[3] != NULL) {
		glVertex3f(216.0f, 9.0f, -60.0f);
		glVertex3f(224.0f, 9.0f, -60.0f);
		glVertex3f(224.0f, 23.0f, -60.0f);
		glVertex3f(216.0f, 23.0f, -60.0f);
	}
	glColor3ub(255, 255, 255);
	glEnd();
}

void World::draw() {
	unsigned int i;
	glLoadIdentity();

	//DRAW ORDER: BACK --> FRONT
	drawHUD();
	glTranslatef(0.0f, 32.0f, 0.0f);

	//Draw Background
	drawBG();

	//Draw World
	drawWorld();

	//Draw World Shadows
	drawShadows();

	//Draw Entities
	for(i = 0; i < _x * _y; i++) {
		if(ents[i] != NULL) {
			ents[i]->drawShift(32.0f);
		}
	}

	/*
	//Draw NPCs First (Behind Player)
	for(i = 0; i < npcs.size(); i++) {
		npcs[i]->drawShift(16.0f);
	}
	*/

	//Draw Player
	for(i = 0; i < MAXPLAYERS; i++) {
		if(players[i] != NULL) {
			players[i]->draw();
		}
	}

	if(endTriggered) {
		glBindTexture(GL_TEXTURE_2D, 0);
		glColor4ub(0, 0, 0, 50);
		glBegin(GL_QUADS);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(_x * 16.0f, 0.0f, 0.0f);
		glVertex3f(_x * 16.0f, _y * 16.0f, 0.0f);
		glVertex3f(0.0f, _y * 16.0f, 0.0f);
		glEnd();
		glColor4ub(255, 255, 255, 255);
	}

	/*
	for(i = 0; i < MAXPLAYERS; i++) {
		if(players[i] != NULL) {
			players[i]->drawWalk();
		}
	}
	*/

	/*
	//Draw NPCs
	for(i = 0; i < npcs.size(); i++) {
		npcs[i]->draw();
	}
	*/

	//Draw MAPDATA UNSAFE
	/*glBindTexture(GL_TEXTURE_2D, 0);
	glColor4ub(0, 255, 0, 100);
	glBegin(GL_QUADS);
	for(int i = 0; i < _x * _y; i++) {
		int x = i % _x;
		int y = i / _x;
		if(mapData[i] & World::MAPDATA_UNSAFE) {
			glVertex3f(x * 16.0f, y * 16.0f, 0.0f);
			glVertex3f(x * 16.0f + 16.0f, y * 16.0f, 0.0f);
			glVertex3f(x * 16.0f + 16.0f, y * 16.0f + 16.0f, 0.0f);
			glVertex3f(x * 16.0f, y * 16.0f + 16.0f, 0.0f);
		}
	}
	glColor4ub(255, 255, 255, 255);
	glEnd();*/
}

void World::update() {
	unsigned int i;

	//Update Map Data (For AI & Player Movement)
	updateMapData();

	//Update Player
	for(i = 0; i < MAXPLAYERS; i++) {
		if(players[i] != NULL) {
			players[i]->think();
			players[i]->update();
		}
	}

	/*
	//Update NPCS
	for(PlayerNPC* npc : npcs) {
		npc->think();
		npc->update();
	}
	*/

	//Update Ents
	for(i = 0; i < _x * _y; i++) {
		if(ents[i] != NULL) {
			ents[i]->update();
		}
	}

	//Update Music??
	static bool test = false;
	static int test2 = 600;
	if(!test) {
		PlaySound("sound/music/bmode_loop.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC );
		test = true;
	}
	if(test2 > 0) {
		test2--;
	} else if(test2 == 0) {
		//cout << "STOPPING" << endl;
		//sndPlaySound((LPCSTR)NULL, SND_ASYNC);
		//test2--;
	}

	if(!endTriggered) {
		int alivePlayers = 0;
		for(unsigned int i = 0; i < MAXPLAYERS; i++) {
			if(players[i] != NULL && players[i]->isAlive()) {
				alivePlayers++;
			}
		}
		if(alivePlayers == 1) {
			//WINNER
			endTriggered = true;
			for(unsigned int i = 0; i < MAXPLAYERS; i++) {
				if(players[i] != NULL && players[i]->isAlive()) {
					players[i]->score++;
					break;
				}
			}
			endType = 1;
			endCounter = 3 * 60;
		} else if(alivePlayers == 0) {
			//DRAW
			endTriggered = true;
			endType = 2;
			endCounter = 3 * 60;
		} else if(timer <= 0) {
			endTriggered = true;
			endType = 0;
			endCounter = 3 * 60;
		}
	} else {
		endCounter--;
		if(endCounter <= 0) {
			restart();
			endTriggered = false;
		}
	}

	timer--;
}

int World::xyToIndex(unsigned int x, unsigned int y) {
	return y * _x + x;
}

void World::drawWorld() {
	static float texSize = 16.0f / 64.0f;
	unsigned int i, j;
	unsigned int currBlockType = 0;
	float texX, texY;
	//TEMP
	float w, h;
	static GLuint texID = TextureHandler::loadTexture("textures/default/world/worldset.png", w, h);
	glBindTexture(GL_TEXTURE_2D, texID);
	glBegin(GL_QUADS);
	for(i = 0; i < _x; i++) {
		for(j = 0; j < _y; j++) {
			unsigned char blockType = blockData[xyToIndex(i,j)];
			if(blockType != 0) {
				if(blockType != currBlockType) {
					//glEnd();
					//texID = TextureHandler::loadTexture(blockList[blockType].texture);
					texX = (blockList[blockType].texX * 16.0f) / 64.0f;
					texY = (blockList[blockType].texY * 16.0f) / 64.0f;
					currBlockType = blockType;
					//glBindTexture(GL_TEXTURE_2D, texID);
					//glBegin(GL_QUADS);
				}
				float x = i * 16.0f;
				float y = j * 16.0f;
				glTexCoord2f(texX, texY);
				glVertex3f(x, y, -200.0f);
				glTexCoord2f(texX + texSize, texY);
				glVertex3f(x + 16.0f, y, -200.0f);
				glTexCoord2f(texX + texSize, texY + texSize);
				glVertex3f(x + 16.0f, y + 16.0f, -200.0f);
				glTexCoord2f(texX, texY + texSize);
				glVertex3f(x, y + 16.0f, -200.0f);
			}
		}
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void World::drawBG() {
	glColor3f(0.0625f,0.4705f,0.1882f);
	glBegin(GL_QUADS);
	glVertex3f(0.0f, 0.0f, -255.0f);
	glVertex3f(240.0f, 0.0f, -255.0f);
	glVertex3f(240.0f, 240.0f, -255.0f);
	glVertex3f(0.0f, 240.0f, -255.0f);
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
}

void World::drawShadows() {
	static float texSize = 16.0f / 64.0f;
	//TEMP
	float w, h;
	static GLuint texID = TextureHandler::loadTexture("textures/default/world/worldset.png", w, h);
	glBindTexture(GL_TEXTURE_2D, texID);
	glColor4f(1.0f, 1.0f, 1.0f, 0.33f);
	glBegin(GL_QUADS);
	bool block = false;
	for(unsigned int i = 0; i < _x; i++) {
		for(unsigned int j = 0; j < _y; j++) {
			unsigned short type = mapData[xyToIndex(i, j)] & (MAPDATA_WORLD | MAPDATA_DESTRUCTIBLE);
			//unsigned char type = blockData[xyToIndex(i, j)];
			if(type != 0) {
				block = true;
			} else if(type == 0 && block == true) {
				block = false;
				float x = i * 16.0f;
				float y = j * 16.0f;
				glTexCoord2f(0, 0);
				glVertex3f(x, y, -254.0f);
				glTexCoord2f(texSize, 0);
				glVertex3f(x + 16.0f, y, -254.0f);
				glTexCoord2f(texSize, texSize);
				glVertex3f(x + 16.0f, y + 16.0f, -254.0f);
				glTexCoord2f(0, texSize);
				glVertex3f(x, y + 16.0f, -254.0f);
			}
		}
		block = false;
	}
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnd();
}

void generateBlockList() {
	blockList[BLOCKTYPE_BLANK] = Block(0,0,false);
	blockList[BLOCKTYPE_WALL] = Block(1,0,true);
}

unsigned char* World::getBlockData() {
	return blockData;
}

Block* getBlockList() {
	return blockList;
}

void World::updateMapData() {
	for(unsigned int j = 0; j < _y; j++) {
		for(unsigned int i = 0; i < _x; i++) {
			int index = xyToIndex(i, j);
			//Refresh Map Data (All 0's)
			mapData[index] = 0;

			//Add Collision Data for World
			if(blockList[blockData[index]].collision) {
				mapData[index] |= MAPDATA_BLOCKED | MAPDATA_WORLD; //Add Block
			}
		}
	}

	//Add Entity Dataset
	for(unsigned int i = 0; i < _x * _y; i++) {
		if(ents[i] != NULL) {
			mapData[xyToIndex(ents[i]->getX(), ents[i]->getY())] |= ents[i]->getMapDataMask();
		}
	}

	//Add Bomb Dataset
	for(unsigned int i = 0; i < _x * _y; i++) {
		if(ents[i] != NULL) {
			if(ents[i]->getMapDataMask() & MAPDATA_BOMB) {
				EntityBomb* bomb = (EntityBomb*)ents[i];
				bool contRight = true, contLeft = true, contUp = true, contDown = true;
				for(unsigned int r = 1; r <= bomb->getBlastRadius(); r++) {
					short mData = mapData[shiftIndex(i, r, 0)];
					if(contRight && ((mData & (MAPDATA_BLOCKED | MAPDATA_ENTITY)) == 0)) {
						mapData[shiftIndex(i, r, 0)] |= MAPDATA_UNSAFE;
					} else {
						if(contRight && mData & MAPDATA_ENTITY) {
							mapData[shiftIndex(i, r, 0)] |= MAPDATA_UNSAFE;
						}
						contRight = false;
					}

					mData = mapData[shiftIndex(i, -r, 0)];
					if(contLeft && ((mData & (MAPDATA_BLOCKED | MAPDATA_ENTITY)) == 0)) {
						mapData[shiftIndex(i, -r, 0)] |= MAPDATA_UNSAFE;
					} else {
						if(contLeft && mData & MAPDATA_ENTITY) {
							mapData[shiftIndex(i, -r, 0)] |= MAPDATA_UNSAFE;
						}
						contLeft = false;
					}

					mData = mapData[shiftIndex(i, 0, r)];
					if(contDown && ((mData & (MAPDATA_BLOCKED | MAPDATA_ENTITY)) == 0)) {
						mapData[shiftIndex(i, 0, r)] |= MAPDATA_UNSAFE;
					} else {
						if(contDown && mData & MAPDATA_ENTITY) {
							mapData[shiftIndex(i, 0, r)] |= MAPDATA_UNSAFE;
						}
						contDown = false;
					}

					mData = mapData[shiftIndex(i, 0, -r)];
					if(contUp && ((mData & (MAPDATA_BLOCKED | MAPDATA_ENTITY)) == 0)) {
						mapData[shiftIndex(i, 0, -r)] |= MAPDATA_UNSAFE;
					} else {
						if(contUp && mData & MAPDATA_ENTITY) {
							mapData[shiftIndex(i, 0, -r)] |= MAPDATA_UNSAFE;
						}
						contUp = false;
					}
				}
			}
		}
	}
}

unsigned short* World::getMapData() {
	return mapData;
}

unsigned int World::get_X() {
	return _x;
}

unsigned int World::get_Y() {
	return _y;
}

void World::xyFromNum(int num, float& x, float& y) {
	if(num >= 8) {
		y = (12.0f / 64.0f);
		num -= 8;
	} else {
		y = 0.0f;
	}
	x = (num * 8.0f) / 64.0f;
}

void World::addEntity(Entity* ent) {
	int index = xyToIndex(ent->getX(), ent->getY());
	if(ents[index] != NULL) {
		//cout << "Warning Reference to Entity index " << index << " is begin dropped from Entity List without Removal" << endl;
		ents[index]->~Entity();
	}
	ents[index] = ent;
	mapData[index] = ent->getMapDataMask();
}

Entity* World::getEntity(int x, int y) {
	return ents[xyToIndex(x, y)];
}

void World::removeEntity(Entity* ent) {
	int index = xyToIndex(ent->getX(), ent->getY());
	if(ents[index] == ent) {
		ents[index] = NULL;
	}
}

/*
vector<PlayerNPC*>* World::getNpcs() {
	return &npcs;
}
*/

unsigned int World::shiftIndex(unsigned int index, int x, int y) {
	return index + (y * _x) + x;
}

void World::addPlayer(Player* player) {
	for(int i = 0; i < MAXPLAYERS; i++) {
		if(players[i] == NULL) {
			players[i] = player;
			break;
		}
	}
}

void World::restart() {
	delete[] ents;
	ents = new Entity*[_x * _y];
	for(unsigned int i = 0; i < _x * _y; i++) {
		ents[i] = NULL;
	}
	delete[] blockData;
	blockData = new unsigned char[_x * _y];
	for(unsigned int i = 0; i < MAXPLAYERS; i++) {
		if(players[i] != NULL) {
			players[i]->reset();
		}
	}
	timer = 120 * 60 + 59;
	generateTerrain();

}