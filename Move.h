#pragma once

#include "Piece.h"
#include "Field.h"
#include <list>
#include <sstream>
#include <string>

using namespace std;

class Move {
public:
	Move(Field startField, Field endField);
	Move(char* str);
	~Move();
	string toString();
	Field getStartField();
	Field getEndField();
private:
	Field m_startField;
	Field m_endField;
};

