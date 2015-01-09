#include "Block.h"

Block::Block(void) {
}

Block::Block(unsigned char texX, unsigned char texY, bool collision)
{
	this->texX = texX;
	this->texY = texY;
	this->collision = collision;
}


Block::~Block(void)
{
}
