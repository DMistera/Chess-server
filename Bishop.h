#pragma once

#include "Piece.h"

class Bishop : public Piece
{
public:
	Bishop(Side side, int x);
	~Bishop();

	// Inherited via Piece
	virtual std::list<Field> getAvailableFields(GameState & state) override;

	// Inherited via Piece
	virtual Type getType() override;
};

