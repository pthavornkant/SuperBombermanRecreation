#include "PlayerNPC.h"
#include "GameWindow.h"
#include <queue>
#include <math.h>
#include <functional>

#include <iostream>
using namespace std;

int PlayerNPC::DEFAULTDELAY = 20;
int PlayerNPC::GOALSTEPSIZE = 3;

PlayerNPC::PlayerNPC(void) : Player(1, 1, 1) {
	cout << "Warning! PlayerNPC::PlayerNPC() Default Constructor Used" << endl;
	centerX = 8.0f;
	centerY = 16.0f;
	width = 16.0f;
	height = 24.0f;
	goalAchieved = true;
	//setTexture("textures/default/char/bomberman_bw.png");
	//walkMap = new bool[GameWindow::world->get_X() * GameWindow::world->get_Y()];
	walkMap = new list<int>();
	delay = DEFAULTDELAY;
	pMap = new bool[GameWindow::world->get_X() * GameWindow::world->get_Y()];
	goalOffset = 0;
	goalProcedure = NULL;
	blockCounter = 0;
}

PlayerNPC::PlayerNPC(float x, float y, int playerNum) : Player(x, y, playerNum) {
	centerX = 8.0f;
	centerY = 16.0f;
	width = 16.0f;
	height = 24.0f;
	goalAchieved = true;
	//setTexture("textures/default/char/bomberman_bw.png");
	//walkMap = new bool[GameWindow::world->get_X() * GameWindow::world->get_Y()];
	walkMap = new list<int>();
	delay = DEFAULTDELAY;
	pMap = new bool[GameWindow::world->get_X() * GameWindow::world->get_Y()];
	goalOffset = 0;
	goalProcedure = NULL;
	blockCounter = 0;
}


PlayerNPC::~PlayerNPC(void) {
}

void PlayerNPC::think() {
	/*
	if(goalAchieved) {
		int dir = rand() % 5;
		keyForward = keyBackward = keyLeft = keyRight = keyBomb = false;
		if(dir == 0) {
			keyForward = true;
		} else if(dir == 1) {
			keyBackward = true;
		} else if(dir == 2) {
			keyLeft = true;
		} else if(dir == 3) {
			keyRight = true;
		} else if(dir == 4) {
			keyBomb = true;
		}
		goalAchieved = false;
		delay = DEFAULTDELAY;
	} else {
		delay--;
		if(delay <= 0) {
			goalAchieved = true;
			delay = DEFAULTDELAY;
		}
	}
	*/
}

void PlayerNPC::draw() {
	Entity::draw();

}

void PlayerNPC::update() {
	//Character NPC AI
	World* world = GameWindow::world;
	unsigned int cx = (int)(x + 0.5f);
	unsigned int cy = (int)(y + 0.5f);
	unsigned int currentIndex = world->xyToIndex(cx, cy);
	if(goalAchieved) {
		unsigned int mapX = world->get_X();
		unsigned int mapY = world->get_Y();
		const unsigned int maxIndex = mapX * mapY;
		unsigned short *mapData = world->getMapData();

		bool currentUnsafe = mapData[currentIndex] & World::MAPDATA_UNSAFE;

		//Processed Map
		for(unsigned int i = 0; i < maxIndex; i++) {
			pMap[i] = 0;
		}

		//Clear Walkmap
		walkMap->clear();
		//Generate Walkmap
		queue<pair<int,bool>> processQueue = queue<pair<int, bool>>();
		processQueue.push(pair<int, bool>(world->xyToIndex(cx, cy), currentUnsafe));
		while(!processQueue.empty()) {
			pair<int, bool> curPair = processQueue.front();
			int index = curPair.first;
			bool unsafe = curPair.second;
			if(unsafe && (mapData[index] & World::MAPDATA_UNSAFE == 0)) {
				unsafe = false;
			}
			if(pMap[index] == 0) {
				//Shift Right
				int nIndex = world->shiftIndex(index, 1, 0);
				if(!(mapData[nIndex] & World::MAPDATA_BLOCKED)) {
					if(!unsafe && (mapData[nIndex] & World::MAPDATA_UNSAFE)) {
						//Ignore this Block
					} else {
						processQueue.push(pair<int, bool>(nIndex, unsafe));
					}
				}

				//Shift Left
				nIndex = world->shiftIndex(index, -1, 0);
				if(!(mapData[nIndex] & World::MAPDATA_BLOCKED)) {
					if(!unsafe && (mapData[nIndex] & World::MAPDATA_UNSAFE)) {
						//Ignore this Block
					} else {
						processQueue.push(pair<int, bool>(nIndex, unsafe));
					}
				}

				//Shift Down
				nIndex = world->shiftIndex(index, 0, 1);
				if(!(mapData[nIndex] & World::MAPDATA_BLOCKED)) {
					if(!unsafe && (mapData[nIndex] & World::MAPDATA_UNSAFE)) {
						//Ignore this Block
					} else {
						processQueue.push(pair<int, bool>(nIndex, unsafe));
					}
				}

				//Shift Up
				nIndex = world->shiftIndex(index, 0, -1);
				if(!(mapData[nIndex] & World::MAPDATA_BLOCKED)) {
					if(!unsafe && (mapData[nIndex] & World::MAPDATA_UNSAFE)) {
						//Ignore this Block
					} else {
						processQueue.push(pair<int, bool>(nIndex, unsafe));
					}
				}
				walkMap->push_back(index);
			}
			pMap[index] = 1;
			processQueue.pop();
		}

		this->mapData = mapData;

		if(bombSupply > 0 && (rand() % 100) < 75) {
			//Drop a Bomb OR Do Something Else
			if((rand() % 100) < 50) {
				//Attack Player OR Destroy Destructibles
				if((rand() % 100) < 50) {
					attackPlayers();
				} else {
					destroyBlocks();
				}
			} else {
				//Grab an Entity OR Avoid Unsafe Areas
				if((rand() % 100) < 75) {
					seekEntity();
				} else {
					avoidUnsafe();
				}
			}
		} else {
			//Grab Entites OR Avoid Unsafe Areas
			if((rand() % 100) < 75) {
				seekEntity();
			} else {
				avoidUnsafe();
			}
		}
	} else {
		keyLeft = keyRight = keyForward = keyBackward = keyBomb = false;
		if(goalProcedure != NULL) {
			char x = goalProcedure[goalOffset * GOALSTEPSIZE];
			if(x == -1) {
				goalAchieved = true;
				goalOffset = 0;
				delete[] goalProcedure;
			} else {
				char y = goalProcedure[(goalOffset * GOALSTEPSIZE) + 1];
				char spec = goalProcedure[(goalOffset * GOALSTEPSIZE) + 2];

				//cout << "X = " << (int)x << " && y = " << (int)y << " && spec = " << (int)spec << endl;

				if(x > cx) { 
					keyRight = true;
				} else if(x < cx) {
					keyLeft = true;
				} else if(y < cy) {
					keyForward = true;
				} else if(y > cy) {
					keyBackward = true;
				} else {
					if(spec & GOAL_DROPBOMB) {
						keyBomb = true;
					}
					goalOffset++;
				}
			}
		}
	}

	int oldX = this->x, oldY = this->y;
	Character::update();
	if(this->x == oldX && this->y == oldY) {
		blockCounter++;
	} else {
		blockCounter = 0;
	}
	if(blockCounter > 15) {
		goalAchieved = true;
		goalOffset = 0;
		goalProcedure = NULL;
		blockCounter = 0;
	}
}

/*
struct NodeCompare : public std::binary_function<

void PlayerNPC::generatePath(int x, int y) {
	priority_queue<int, vector<int>, 
}
*/

void PlayerNPC::handleEvent(const GlutWindow::Event& e) {
	//Do Not Respond to Keyboard Events
}

void PlayerNPC::attackPlayers() {
	World* world = GameWindow::world;
	float distance[world->MAXPLAYERS];
	bool totalReachable = false;
	for(unsigned int i = 1; i <= world->MAXPLAYERS; i++) {
		Player* player = world->getPlayer(i);
		//Don't Target Yourself!
		if(player == this || !player->isAlive()) {
			distance[i-1] = -1.0f;
			continue;
		}
		unsigned int locIndex = world->xyToIndex(player->getX(), player->getY());
		bool reachable = false;
		for(int index : (*walkMap)) {
			if(locIndex == index) {
				reachable = true;
				totalReachable = true;
				break;
			}
		}
		if(reachable) {
			float dx = x - player->getX();
			float dy = y - player->getY();
			distance[i-1] = sqrt(dx * dx + dy * dy);
		} else {
			distance[i-1] = -1.0f;
		}
	}

	//If Nobody is Reachable, Forward to Destroy Blocks
	if(!totalReachable) {
		destroyBlocks(true);
		return;
	}

	//Attack a Player
	//(For Now, just Attack the Closest)
	float min = distance[0];
	int player = 0;
	for(int i = 1; i < world->MAXPLAYERS; i++) {
		if((distance[i] < min && distance[i] != -1.0f) || min == -1.0f) {
			player = i;
			min = distance[i];
		}
	}
	player++; //Shift back to world player #

	Player* target = world->getPlayer(player);

	//For Now, Try to Drop Bomb on Player
	unsigned int tIndex = world->xyToIndex(target->getX(), target->getY());
	setGoal(target->getX(), target->getY(), GOAL_DROPBOMB);
}

void PlayerNPC::seekEntity() {
	World* world = GameWindow::world;
	vector<int>* entInd = new vector<int>();
	for(int index : (*walkMap)) {
		if(mapData[index] & (World::MAPDATA_ENTITY)) {
			if(mapData[index] & (World::MAPDATA_BOMB | World::MAPDATA_DESTRUCTIBLE) == 0) {
				entInd->push_back(index);
			}
		}
	}
	
	if(!entInd->empty()) {
		//For Now, seek first entity found
		unsigned int index = entInd->front();
		Entity* ent = world->getEntity(index % world->get_X(), index / world->get_Y());
		setGoal(ent->getX(), ent->getY());
	} else {
		avoidUnsafe();
	}
}

void PlayerNPC::avoidUnsafe() {
	World* world = GameWindow::world;
	int cx = (int)(x + 0.5f);
	int cy = (int)(y + 0.5f);
	unsigned int currIndex = world->xyToIndex(cx, cy);
	int bestSafe = -1;
	float bestDistance = -1.0f;
	if(mapData[currIndex] & World::MAPDATA_UNSAFE) {
		for(int index : (*walkMap)) {
			if((mapData[index] & World::MAPDATA_UNSAFE) == 0) {
				int dx = index % world->get_X() - currIndex % world->get_X();
				int dy = index / world->get_X() - currIndex / world->get_X();
				float distance = sqrt(dx * dx + dy * dy);
				if(bestDistance == -1.0f || distance < bestDistance) {
					bestDistance = distance;
					bestSafe = index;
				}
			}
		}
		setGoal(bestSafe % world->get_X(), bestSafe / world->get_X());
	} else {
		//Do nothing, we are safe.
	}
}

void PlayerNPC::destroyBlocks(bool force) {
	World* world = GameWindow::world;
	int bestIndex = -1;
	int bestTotalHit = -1;
	bool *walkBitMap = new bool[world->get_X() * world->get_Y()];
	int totalSpots = 0;
	for(int i = 0; i < world->get_X() * world->get_Y(); i++) {
		walkBitMap[i] = 0;
	}
	for(int index : (*walkMap)) {
		walkBitMap[index] = 1;
		totalSpots++;
	}
	for(int index : (*walkMap)) {
		int x = index % world->get_X();
		int y = index / world->get_Y();

		int totalHit = 0;
		int intTotalSpots = 1; //Initial Bomb Drop Spot

		bool contLeft = true, contRight = true, contUp = true, contDown = true;
		for(int r = 1; r <= bombBlast && (contLeft | contRight | contUp | contDown); r++) {
			if(contLeft) {
				int shiftIndex = world->shiftIndex(index, -r, 0);
				if(mapData[shiftIndex] & (World::MAPDATA_BLOCKED | World::MAPDATA_ENTITY)) {
					if(mapData[shiftIndex] & World::MAPDATA_DESTRUCTIBLE) {
						totalHit++;
					}
					contLeft = false;
				} else {
					intTotalSpots++;
				}
			}

			if(contRight) {
				int shiftIndex = world->shiftIndex(index, r, 0);
				if(mapData[shiftIndex] & (World::MAPDATA_BLOCKED | World::MAPDATA_ENTITY)) {
					if(mapData[shiftIndex] & World::MAPDATA_DESTRUCTIBLE) {
						totalHit++;
					}
					contRight = false;
				} else {
					intTotalSpots++;
				}
			}

			if(contUp) {
				int shiftIndex = world->shiftIndex(index, 0, -r);
				if(mapData[shiftIndex] & (World::MAPDATA_BLOCKED | World::MAPDATA_ENTITY)) {
					if(mapData[shiftIndex] & World::MAPDATA_DESTRUCTIBLE) {
						totalHit++;
					}
					contUp = false;
				} else {
					intTotalSpots++;
				}
			}

			if(contDown) {
				int shiftIndex = world->shiftIndex(index, 0, r);
				if(mapData[shiftIndex] & (World::MAPDATA_BLOCKED | World::MAPDATA_ENTITY)) {
					if(mapData[shiftIndex] & World::MAPDATA_DESTRUCTIBLE) {
						totalHit++;
					}
					contDown = false;
				} else {
					intTotalSpots++;
				}
			}
		}
		if(totalHit > bestTotalHit && intTotalSpots < totalSpots) {
			bestTotalHit = totalHit;
			bestIndex = index;
			if(bestTotalHit >= 3) {
				break;
			}
		}
	}
	delete[] walkBitMap;
	if(bestIndex == -1) {
		avoidUnsafe();
	} else {
		setGoal(bestIndex % world->get_X(), bestIndex / world->get_X(), GOAL_DROPBOMB);
	}
}

void PlayerNPC::setGoal(char x, char y, char spec) {
	World* world = GameWindow::world;
	int cx = (int)(this->x + 0.5f);
	int cy = (int)(this->y + 0.5f);
	int cIndex = cy * world->get_X() + cx;

	goalAchieved = false;
	goalOffset = 0;
	unsigned char* goal = new unsigned char[100];

	int maxIndex = world->get_X() * world->get_Y();
	WalkNode **spreadMap = new WalkNode*[maxIndex];
	for(int i = 0; i < maxIndex; i++) {
		spreadMap[i] = NULL;
	}
	WalkNode* headNode = new WalkNode(-1, 0, cIndex);
	spreadMap[headNode->currIndex] = headNode;
	priority_queue<WalkNode*, vector<WalkNode*>, MyComparator> processingQueue = priority_queue<WalkNode*, vector<WalkNode*>, MyComparator>();
	processingQueue.push(headNode);
	vector<WalkNode*> finishedQueue = vector<WalkNode*>();
	bool routeFound = false;
	WalkNode* tailNode = NULL;
	//int DEBUGITERATIONS = 0;
	while(!processingQueue.empty() && !routeFound) {
		WalkNode* cNode = processingQueue.top();
		processingQueue.pop();
		//DEBUGITERATIONS++;
		//cout << "COMPLEXITY: " << cNode->complexity << endl;

		int cx = cNode->currIndex % world->get_X();
		int cy = cNode->currIndex / world->get_X();
		int cIndex = cNode->currIndex;
		int comp = cNode->complexity;
		int prevIndex = cNode->prevIndex;

		if(cx == x && cy == y) {
			routeFound = true;
			tailNode = cNode;
			break;
		}

		int plusComp = 10;

		//Right Shift
		int nIndex = world->shiftIndex(cIndex, 1, 0);
		//cout << "SPREADMAP = " << hex << spreadMap[nIndex] << endl;
		//cout << "MAPDATA = " << hex << (mapData[nIndex] & World::MAPDATA_BLOCKED) << endl;
		if(((mapData[nIndex] & World::MAPDATA_BLOCKED) == 0) && spreadMap[nIndex] == NULL) {
			if(mapData[nIndex] & World::MAPDATA_UNSAFE) {
				plusComp = 100;
			} else {
				plusComp = 10;
			}
			WalkNode* nextNode = new WalkNode(cIndex, comp + plusComp, nIndex);
			processingQueue.push(nextNode);
			spreadMap[nIndex] = nextNode;
		}

		//Left Shift
		nIndex = world->shiftIndex(cIndex, -1, 0);
		//cout << "SPREADMAP = " << hex << spreadMap[nIndex] << endl;
		//cout << "MAPDATA = " << hex << (mapData[nIndex] & World::MAPDATA_BLOCKED) << endl;
		//cout << "TRUE? = " << ((mapData[nIndex] & World::MAPDATA_BLOCKED) == 0) << endl;
		//cout << "TRUE? = " << (spreadMap[nIndex] == NULL) << endl;
		if(((mapData[nIndex] & World::MAPDATA_BLOCKED) == 0) && spreadMap[nIndex] == NULL) {
			if(mapData[nIndex] & World::MAPDATA_UNSAFE) {
				plusComp = 100;
			} else {
				plusComp = 10;
			}
			WalkNode* nextNode = new WalkNode(cIndex, comp + plusComp, nIndex);
			processingQueue.push(nextNode);
			spreadMap[nIndex] = nextNode;
		}

		//Up Shift
		nIndex = world->shiftIndex(cIndex, 0, -1);
		//cout << "SPREADMAP = " << hex << spreadMap[nIndex] << endl;
		//cout << "MAPDATA = " << hex << (mapData[nIndex] & World::MAPDATA_BLOCKED) << endl;
		if(((mapData[nIndex] & World::MAPDATA_BLOCKED) == 0) && spreadMap[nIndex] == NULL) {
			if(mapData[nIndex] & World::MAPDATA_UNSAFE) {
				plusComp = 100;
			} else {
				plusComp = 10;
			}
			WalkNode* nextNode = new WalkNode(cIndex, comp + plusComp, nIndex);
			processingQueue.push(nextNode);
			spreadMap[nIndex] = nextNode;
		}

		//Down Shift
		nIndex = world->shiftIndex(cIndex, 0, 1);
		//cout << "SPREADMAP = " << hex << spreadMap[nIndex] << endl;
		//cout << "MAPDATA = " << hex << (mapData[nIndex] & World::MAPDATA_BLOCKED) << endl;
		if(((mapData[nIndex] & World::MAPDATA_BLOCKED) == 0) && spreadMap[nIndex] == NULL) {
			if(mapData[nIndex] & World::MAPDATA_UNSAFE) {
				plusComp = 100;
			} else {
				plusComp = 10;
			}
			WalkNode* nextNode = new WalkNode(cIndex, comp + plusComp, nIndex);
			processingQueue.push(nextNode);
			spreadMap[nIndex] = nextNode;
		}

		finishedQueue.push_back(cNode);
	}
	//routeFound = true;
	//cout << endl << endl;

	//cout << "Iterations: " << DEBUGITERATIONS << endl;

	if(routeFound) {
		//cout << "ROUTING SUCCESS" << endl;
		WalkNode* cNode = tailNode;
		while(cNode != headNode) {
			//cout << "HEAD IND = " << headNode->currIndex << endl;
			//cout << "IND = " << cNode->currIndex << " --> " << cNode->prevIndex << endl;
			WalkNode* pNode = spreadMap[cNode->prevIndex];
			pNode->nextIndex = cNode->currIndex;
			cNode = pNode;
		}

		//cout << "CX = " << cx << " && CY = " << cy << endl;

		cNode = headNode;
		int cDir = -1;
		int pDir = -1;
		int goalOff = 0;
		while(cNode != tailNode) {
			int dx = (cNode->nextIndex % world->get_X() - cNode->currIndex % world->get_X());
			int dy = (cNode->nextIndex / world->get_X() - cNode->currIndex / world->get_X());
			pDir = cDir;
			if(dx < 0) {
				cDir = 1;
			} else if(dx > 0) {
				cDir = 2;
			} else if(dy < 0) {
				cDir = 3;
			} else {
				cDir = 4;
			}

			if(pDir != cDir) {
				goal[(goalOff * GOALSTEPSIZE)] = cNode->currIndex % world->get_X();
				goal[(goalOff * GOALSTEPSIZE) + 1] = cNode->currIndex / world->get_X();
				goal[(goalOff * GOALSTEPSIZE) + 2] = 0;
				//cout << "ADDED STEP x = " << (int)goal[goalOff * GOALSTEPSIZE] << " && y = " << (int)goal[goalOff * GOALSTEPSIZE + 1] << " && spec = " << (int)goal[goalOff * GOALSTEPSIZE + 2] << endl;
				goalOff++;
			}
			//cout << "DX = " << dx << " && DY = " << dy << endl;
			cNode = spreadMap[cNode->nextIndex];
		}

		//cout << "GOAL OFFSET = " << goalOff << endl;
		goal[(goalOff * GOALSTEPSIZE)] = x;
		goal[(goalOff * GOALSTEPSIZE + 1)] = y;
		goal[(goalOff * GOALSTEPSIZE + 2)] = spec;
		goal[(++goalOff * GOALSTEPSIZE)] = -1;
		//Walk Forward through the List & Path to Goal
		//int direction = -1;
	} else {
		//cout << "ROUTING FAILURE" << endl;
		goal[0] = cx;
		goal[1] = cy;
		goal[2] = 0;
		goal[3] = -1;
	}

	//cout << "CX = " << cx << endl << "CY = " << cy << endl;
	//cout << "-->NX = " << (int)x << endl << "-->CY = " << (int)y << endl;

	goalProcedure = goal;
}

void PlayerNPC::drawWalk() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);
	glColor4ub(255,0,0,100);
	for(int index : (*walkMap)) {
		float x = index % 15 * 16.0f;
		float y = index / 15 * 16.0f;
		glVertex3f(x, y, -1.0f);
		glVertex3f(x + 16.0f, y, -1.0f);
		glVertex3f(x + 16.0f, y + 16.0f, -1.0f);
		glVertex3f(x, y + 16.0f, -1.0f);
	}
	glColor4ub(255,255,255,255);
	glEnd();
}