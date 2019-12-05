#pragma once

#include "Piece.h"

class Rook : public Piece
{
public:
	Rook(Side, int x);
	~Rook();

	// Inherited via Piece
	virtual std::list<Field> getAvailableFields(GameState& state) override;

	// Inherited via Piece
	virtual Type getType() override;
};

