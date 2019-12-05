#include "Field.h"



Field::Field(int x, int y) {
	this->x = x;
	this->y = y;
}

Field::Field() {
}


Field::~Field() {
}

void Field::reverse()
{
	y = 7 - y;
}

bool Field::inBounds()
{
	return x >= 0 && x <= 7 && y >= 0 && y <= 7;
}
