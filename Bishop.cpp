#include "Bishop.h"
#include "GameState.h"


Bishop::Bishop(Side side, int x) : Piece(side, side == Side::WHITE ? Field(x, 0) : Field(x, 7))
{
}


Bishop::~Bishop()
{
}


std::list<Field> Bishop::getAvailableFields(GameState & state)
{
	std::list<Field> result;
	searchLine(result, state, 1, 1);
	searchLine(result, state, -1, 1);
	searchLine(result, state, -1, -1);
	searchLine(result, state, 1, -1);
	return result;
}

Piece::Type Bishop::getType()
{
	return Type::BISHOP;
}
