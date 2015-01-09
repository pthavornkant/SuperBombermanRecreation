#pragma once

#ifndef _WORLD_H_
#define _WORLD_H_

#include <vector>
#include <map>
#include "Player.h"
#include "Entity.h"
#include "PlayerNPC.h"
#include "Block.h"

using namespace std;

class World
{
public:
	//Max Players
	static const int MAXPLAYERS = 4;

private:
	//Entities, Characters, and Player
	Player* players[MAXPLAYERS];
	//vector<PlayerNPC*> npcs;
	Entity** ents;

	//Counter
	int timer;

	//World Data
	unsigned int _x, _y;
	unsigned char* blockData;
	bool endTriggered;
	int endCounter;
	char endType;

	//World Data for NPCs/Players
	unsigned short* mapData;
	//Update Map Data (For AI and Player)
	void updateMapData();

	//Helper Function for World Draw
	void drawWorld();
	void drawBG();
	void drawShadows();
	void xyFromNum(int num, float& x, float& y);

public:
	World(void);
	World(int x, int y);
	~World(void);

	//0x1 = Blocked
	static const short MAPDATA_BLOCKED = 0x1;
	//0x2 = Kill
	static const short MAPDATA_KILL = 0x2;
	//0x4 = Unsafe (Bomb will hit soon?)
	static const short MAPDATA_UNSAFE = 0x4;
	//0x8 = IsBomb (For Kicking Ability?)
	static const short MAPDATA_BOMB = 0x8;
	//0x10 = Destructible
	static const short MAPDATA_DESTRUCTIBLE = 0x10;
	//0x20 = Entity
	static const short MAPDATA_ENTITY = 0x20;
	//0x40 = World
	static const short MAPDATA_WORLD = 0x40;
	//0x80 = Fire
	static const short MAPDATA_FIRE = 0x80;

	//Entity, Character, and Player Functions
	Player* getPlayer(int num);
	Entity** getEntities();
	//vector<PlayerNPC*>* getNpcs();
	void addEntity(Entity* ent);
	Entity* getEntity(int x, int y);
	void removeEntity(Entity* ent);
	void addPlayer(Player* player);

	//Get Map Data (For AI and Players)
	unsigned short* getMapData();

	//World Data Functions
	void setBlock(unsigned char type, unsigned int& x, unsigned int& y);
	unsigned char getBlock(unsigned int& x, unsigned int& y);
	void generateTerrain();
	unsigned char* getBlockData();
	unsigned int get_X();
	unsigned int get_Y();

	//Helper Function for X & Y to Index Conversion
	int xyToIndex(unsigned int x, unsigned int y);
	unsigned int shiftIndex(unsigned int index, int x, int y);

	//Game Functions
	void drawHUD();
	void draw();
	void update();
	void restart();
};

enum BLOCKTYPE {
	BLOCKTYPE_BLANK,
	BLOCKTYPE_WALL
};

static World* GLOBWORLD;
static Block blockList[256];
void generateBlockList();
Block* getBlockList();

#endif