#include "Queen.h"
#include "GameState.h"


Queen::Queen(Side side) : Piece(side, side == Side::WHITE ? Field(3, 0) : Field(3, 7))
{
}


Queen::~Queen()
{
}


std::list<Field> Queen::getAvailableFields(GameState & state)
{
	std::list<Field> result;
	searchLine(result, state, 1, 1);
	searchLine(result, state, -1, 1);
	searchLine(result, state, -1, -1);
	searchLine(result, state, 1, -1);
	searchLine(result, state, 1, 0);
	searchLine(result, state, -1, 0);
	searchLine(result, state, 0, 1);
	searchLine(result, state, 0, -1);
	return result;
}

Piece::Type Queen::getType()
{
	return Type::QUEEN;
}
