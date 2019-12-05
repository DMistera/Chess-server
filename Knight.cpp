#include "Knight.h"
#include "GameState.h"


Knight::Knight(Side side, int x) : Piece(side, side == Side::WHITE ? Field(x, 0) : Field(x, 7))
{
}


Knight::~Knight()
{
}


std::list<Field> Knight::getAvailableFields(GameState & state)
{
	std::list<Field> result;
	Field f[8];
	f[0] = Field(m_field.x + 1, m_field.y + 2);
	f[1] = Field(m_field.x + 1, m_field.y - 2);
	f[2] = Field(m_field.x + 2, m_field.y + 1);
	f[3] = Field(m_field.x + 2, m_field.y - 1);
	f[4] = Field(m_field.x - 1, m_field.y + 2);
	f[5] = Field(m_field.x - 1, m_field.y - 2);
	f[6] = Field(m_field.x - 2, m_field.y + 1);
	f[7] = Field(m_field.x - 2, m_field.y - 1);
	for (int i = 0; i < 8; i++) {
		if (f[i].inBounds()) {
			Piece* p = state.getPieceAtField(f[i]);
			if (p == 0 || p->getSide() != m_side) {
				result.push_back(f[i]);
			}
		}
	}
	return result;
}

Piece::Type Knight::getType()
{
	return Type::KNIGHT;
}
