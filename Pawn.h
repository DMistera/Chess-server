#pragma once

#include "Piece.h"

class Pawn : public Piece {
public:
	Pawn(Side side, int x);
	~Pawn();

	// Inherited via Piece
	virtual std::list<Field> getAvailableFields(GameState& state) override;

	// Inherited via Piece
	virtual Type getType() override;
};

