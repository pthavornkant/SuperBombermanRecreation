#pragma once

#include "Player.h"
#include <list>
#include <iostream>

using namespace std;

class PlayerNPC : public Player {
	static int DEFAULTDELAY;
	static int GOALSTEPSIZE;

	static const unsigned char GOAL_DROPBOMB = 0x1;

	unsigned char* goalProcedure;
	int goalOffset;
	bool goalAchieved;
	list<int>* walkMap;
	bool *pMap;
	unsigned short* mapData;
	int delay;
	int blockCounter;

public:
	struct WalkNode {
		WalkNode(int pIndex, int complex, int index, int nComplex = -1, int nIndex = -1) : prevIndex(pIndex), complexity(complex), currIndex(index), nextIndex(nIndex) {}
		int prevIndex;
		int complexity;
		int currIndex;
		int nextIndex;
	};
	struct MyComparator {
		bool operator() (WalkNode* left, WalkNode* right) {
			return left->complexity > right->complexity;
		}
	};

	PlayerNPC(void);
	PlayerNPC(float x, float y, int playerNum);
	~PlayerNPC(void);

	void draw();
	void drawWalk();
	void handleEvent(const GlutWindow::Event& e);
	void think();
	void generatePath(int x, int y);
	void update();

	//Actions
	void attackPlayers();
	void seekEntity();
	void avoidUnsafe();
	void destroyBlocks(bool force = false);
	void setGoal(char x, char y, char spec = 0);
};