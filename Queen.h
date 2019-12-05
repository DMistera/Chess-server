#pragma once
#include "Piece.h"

class Queen:
	public Piece
{
public:
	Queen(Side side);
	~Queen();

	// Inherited via Piece
	virtual std::list<Field> getAvailableFields(GameState & state) override;

	// Inherited via Piece
	virtual Type getType() override;
};

