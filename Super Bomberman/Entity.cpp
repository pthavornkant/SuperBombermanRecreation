#include "Entity.h"
#include "World.h"
#include "GameWindow.h"

#include <iostream>
using namespace std;

#define TEXTURE_ENT_DEFAULT "textures/default/ent/Default_Entity.png"

char Entity::animEntityDestruct[] = {8,0,0,0,0,1,0,2,0,3,1,0,1,1,-1};

Entity::Entity(void) {
	cout << "Warning! Entity::Entity() Default Constructor Used" << endl;
	this->x = this->y = 0;
	this->vx = this->vy = 0;
	this->w = this->h = 16;
	centerX = centerY = 8.0f;
	width = height = 16.0f;
	depth = -16.0f;
	setTexture(TEXTURE_ENT_DEFAULT);
	mapDataMask = World::MAPDATA_ENTITY;
	animFunc = defaultAnimFunc;
	destructing = false;
	visible = true;
	entType = 0;
}

Entity::Entity(float x, float y) {
	this->x = x; this->y = y;
	this->vx = this->vy = 0;
	this->w = this->h = 16;
	centerX = centerY = 8.0f;
	width = height = 16.0f;
	depth = -16.0f;
	setTexture(TEXTURE_ENT_DEFAULT);
	mapDataMask = World::MAPDATA_DESTRUCTIBLE | World::MAPDATA_ENTITY;
	animFunc = defaultAnimFunc;
	destructing = false;
	visible = true;
	entType = 0;
}

Entity::Entity(float x, float y, float w, float h) {
	this->x = x; this->y = y;
	this->vx = this->vy = 0;
	this->w = w; this->h = h;
	centerX = centerY = 8.0f;
	width = height = 16.0f;
	depth = -16.0f;
	setTexture(TEXTURE_ENT_DEFAULT);
	mapDataMask = World::MAPDATA_DESTRUCTIBLE | World::MAPDATA_ENTITY;
	animFunc = defaultAnimFunc;
	destructing = false;
	visible = true;
	entType = 0;
}

Entity::~Entity(void) {
	GameWindow::world->removeEntity(this);
}


//Position & Movement Functions
float Entity::getX() {
	return x;
}

float Entity::getY() {
	return y;
}

//Animation Functions
void Entity::setAnimation(char* animData) {
	this->animData = animData + 2;
	animStep = 0;
	animSubStep = 0;
	animDelay = *(animData);
	animRepeat = *(animData + 1);
	animOnce = false;

	//Find Length
	int i;
	for(i = 0; this->animData[i * 2] != -1; i++);
	animLength = i;
}

void Entity::incrAnim() {
	static int test = 0;
	if(animRepeat || !animOnce) {
		animSubStep++;
		if(animSubStep >= animDelay) {
			animSubStep = 0;
			animStep++;
			if(animStep >= animLength) {
				if(animRepeat) {
					animStep = 0;
				} else {
					animStep--;
					animFunc(this);
				}
				animOnce = true;
			}
		}
	}
}

void Entity::getAnimationCoords(char& x, char& y) {
	if(animData) {
		x = animData[animStep * 2 + 1];
		y = animData[animStep * 2];
	}
}

//Texture Functions
void Entity::setTexture(string texture) {
	TextureHandler::unrefTexture(this->texture);
	this->texture = TextureHandler::loadTexture(texture, totalTexWidth, totalTexHeight);
	TextureHandler::refTexture(this->texture);
	dTW = width / totalTexWidth;
	dTH = height / totalTexHeight;
}

void Entity::drawShift(float shift) {
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

void Entity::draw() {
	if(visible) {
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
}

void Entity::update() {
	incrAnim();
}

char Entity::getMapDataMask() {
	return mapDataMask;
}

void defaultAnimFunc(Entity* ent) {
}

void animDestructFunc(Entity* ent) {
	ent->~Entity();
}

void animHideFunc(Entity* ent) {
	ent->setVisible(false);
}

void Entity::destruct() {
	setTexture("textures/default/ent/destruct.png");
	setAnimation(animEntityDestruct);
	animFunc = animDestructFunc;
	mapDataMask |= World::MAPDATA_UNSAFE | World::MAPDATA_KILL;
}

void Entity::setVisible(bool in) {
	visible = in;
}

char Entity::getType() {
	return entType;
}