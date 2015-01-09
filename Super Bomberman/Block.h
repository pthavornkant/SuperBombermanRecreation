#pragma once

#include <string>

using namespace std;

class Block
{
public:
	unsigned char texX, texY;
	int collision;

	Block(void);
	Block(unsigned char texX, unsigned char texY, bool collision);
	~Block(void);
};

