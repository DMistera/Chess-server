#include "King.h"
#include "GameState.h"



King::King(Side side) : Piece(side, side == Side::WHITE ? Field(4, 0) : Field(4, 7)) {
}


King::~King() {
}

std::list<Field> King::getAvailableFields(GameState& state) {
	std::list<Field> result;
	Field f[8];
	f[0] = Field(m_field.x - 1, m_field.y - 1);
	f[1] = Field(m_field.x, m_field.y - 1);
	f[2] = Field(m_field.x + 1, m_field.y - 1);
	f[3] = Field(m_field.x - 1, m_field.y);
	f[4] = Field(m_field.x + 1, m_field.y);
	f[5] = Field(m_field.x - 1, m_field.y + 1);
	f[6] = Field(m_field.x, m_field.y + 1);
	f[7] = Field(m_field.x + 1, m_field.y + 1);
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

Piece::Type King::getType()
{
	return Type::KING;
}
