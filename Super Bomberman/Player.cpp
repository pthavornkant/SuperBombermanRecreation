#include "Player.h"
#include "GameWindow.h"
#include <iostream>
using namespace std;

Player::Player(void) : Character(1, 1) {
	cout << "Warning! Player::Player() Default Constructor Used" << endl;
	centerX = 8.0f;
	centerY = 16.0f;
	width = 16.0f;
	height = 24.0f;
	depth = 0.0f;
	setTexture("textures/default/char/bomberman_white.png");
	score = 0;
	iniX = iniY = 1;
}

Player::Player(float x, float y, int playerNum) : Character(x, y) {
	centerX = 8.0f;
	centerY = 16.0f;
	width = 16.0f;
	height = 24.0f;
	depth = 0.0f;
	if(playerNum == 1) {
		setTexture("textures/default/char/bomberman_white.png");
	} else if(playerNum == 2) {
		setTexture("textures/default/char/bomberman_black.png");
	} else if(playerNum == 3) {
		setTexture("textures/default/char/bomberman_blue.png");
	} else if(playerNum == 4) {
		setTexture("textures/default/char/bomberman_red.png");
	}
	score = 0;
	iniX = x;
	iniY = y;
}

Player::~Player(void) {
}

void Player::handleEvent(const GlutWindow::Event& e) {
	if(e.type == GlutWindow::Keyboard) {
		switch(e.key) {
		case 'w':
			keyForward = true;
			break;
		case 'a':
			keyLeft = true;
			break;
		case 's':
			keyBackward = true;
			break;
		case 'd':
			keyRight = true;
			break;
		case 'c':
			keyBomb = true;
			break;
		case 'x':
			keyBlock = true;
			break;
		}
	} else if(e.type == GlutWindow::KeyboardUp) {
		switch(e.key) {
		case 'w':
			keyForward = false;
			break;
		case 'a':
			keyLeft = false;
			break;
		case 's':
			keyBackward = false;
			break;
		case 'd':
			keyRight = false;
			break;
		case 'c':
			keyBomb = false;
			break;
		case 'x':
			keyBlock = false;
			break;
		}
	}
}

void Player::think() {
	//Do Nothing
}

void Player::drawWalk() {
	//Do Nothing
}

void Player::reset() {
	x = iniX;
	y = iniY;
	dead = false;
	setVisible(true);
	setAnimation(animIdleDown);
	animFunc = defaultAnimFunc;
	PlayerNPC* npc = dynamic_cast<PlayerNPC*>(this);
	if(npc != NULL) {
		npc->setGoal(iniX, iniY, 0);
	}
	pwrup_blast = pwrup_bomb = pwrup_speed = 0;
	bombBlast = DEFAULTBOMBBLAST;
	bombSupply = DEFAULTBOMBSUPPLY;
	speed = DEFAULTSPEED;
}