#pragma once

#include <string>
#include <freeglut.h>
#include "TextureHandler.h"
using namespace std;

class Entity
{
private:
	//Animation Data
	char animStep;
	char animSubStep;
	char animDelay;
	bool animRepeat;
	bool animOnce;
	char animLength;
	char* animData;

	static char animEntityDestruct[];

protected:
	//Position, Movement & Size Data
	float x, y;
	float vx, vy;
	float w, h;
	bool destructing;
	bool visible;
	char entType;

	//Entity Data
	string textureString;
	GLuint texture;
	float centerX, centerY;
	float width, height, depth;
	float totalTexWidth, totalTexHeight;
	float dTW, dTH;
	char mapDataMask;

public:
	Entity(void);
	Entity(float x, float y);
	Entity(float x, float y, float w, float h);
	virtual ~Entity(void);

	//General Direction Data
	static const char DOWN = 0;
	static const char UP = 1;
	static const char LEFT = 2;
	static const char RIGHT = 3;

	//Position & Movement Functions
	float getX();
	float getY();

	//Animation Functions
	void setAnimation(char* animData);
	void incrAnim();
	void getAnimationCoords(char& x, char& y);
	char getMapDataMask();
	void (*animFunc)(Entity* ent);

	//Texture Functions
	void setTexture(string texture);
	void setVisible(bool in);
	char getType();

	//Game Functions
	virtual void draw();
	virtual void drawShift(float shift);
	virtual void update();
	virtual void destruct();
};

void defaultAnimFunc(Entity* ent);
void animDestructFunc(Entity* ent);
void animHideFunc(Entity* ent);