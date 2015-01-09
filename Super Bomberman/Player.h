#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Character.h"
#include "glut_window.h"
#include "TextureHandler.h"

class Player : public Character {
private:
	int iniX, iniY;

public:
	Player(void);
	Player(float x, float y, int playerNum);
	~Player(void);

	char score;

	virtual void handleEvent(const GlutWindow::Event& e);
	virtual void think();
	virtual void drawWalk();
	virtual void reset();
};

#endif