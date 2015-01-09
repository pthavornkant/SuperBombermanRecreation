#pragma once

#include "Entity.h"

class Character : public Entity {
protected:
	static char animIdleUp[], animIdleDown[], animIdleLeft[], animIdleRight[]; //Idle Animation Sequences
	static char animMoveUp[], animMoveDown[], animMoveLeft[], animMoveRight[]; //Movement Animation Sequences
	static char animDeath[]; //Other Animation Sequences
	static int DEFAULTBOMBSUPPLY;
	static int DEFAULTBOMBBLAST;
	static float DEFAULTSPEED;

	bool keyForward, keyBackward, keyLeft, keyRight, keyBomb, keyBlock;

	int bombSupply;
	int bombBlast;
	float speed;

	int prevDirection;
	int prevSwitch;
	bool prevMovement;
	bool dead;
	bool visible;

	static char PWRUP_BOMB_MAX, PWRUP_BLAST_MAX, PWRUP_SPEED_MAX;
	char pwrup_bomb, pwrup_blast, pwrup_speed;

public:
	Character(void);
	Character(float x, float y);
	~Character(void);

	//virtual void drawShift(float shift);
	//virtual void draw();
	virtual void update();
	void addBomb();
	bool isAlive();
};

