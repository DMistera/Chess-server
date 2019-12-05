#pragma once

#include <string>
#include "Field.h"
#include <list>
#include "Side.h"
#include <iostream>

class GameState;

class Piece {
public:
	enum Type {
		PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING
	};

	Piece(Side side, Field field);
	~Piece();
	virtual Type getType() = 0;
	virtual std::list<Field> getAvailableFields(GameState& state) = 0;
	void initializeSprite();
	void setField(Field field);
	Side getSide();
	Field getField();
	static const unsigned int PIECE_SIZE = 50;
protected:
	void searchLine(std::list<Field>& list, GameState& state, int xf, int yf);
	Side m_side;
	Field m_field;
};

