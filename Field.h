#pragma once

class Field {
public:
	Field(int x, int y);
	Field();
	~Field();
	void reverse();
	bool inBounds();
	int x;
	int y;
};

