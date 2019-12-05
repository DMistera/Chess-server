#include "Pawn.h"
#include "GameState.h"

Pawn::Pawn(Side side, int x) : Piece(side, side == Side::WHITE ? Field(x, 1) : Field(x, 6)) {
}

Pawn::~Pawn() {
}


std::list<Field> Pawn::getAvailableFields(GameState& state) {
	std::list<Field> fields;
	Field forward1;
	bool f1 = false;
	Field forward2;
	bool f2 = false;
	if (m_side == Side::WHITE) {
		if (m_field.y < 7) {
			forward1 = Field(m_field.x, m_field.y + 1);
			f1 = true;
			if (m_field.y == 1) {
				forward2 = Field(m_field.x, m_field.y + 2);
				f2 = true;
			}
			for (int i = 0; i < 2; i++) {
				int x = i == 0 ? m_field.x + 1 : m_field.x - 1;
				Field f = Field(x, m_field.y + 1);
				if (state.occupied(f)) {
					fields.push_back(f);
				}
			}
		}
	}
	else {
		if (m_field.y > 0) {
			forward1 = Field(m_field.x, m_field.y - 1);
			f1 = true;
			if (m_field.y == 6) {
				forward2 = Field(m_field.x, m_field.y - 2);
				f2 = true;
			}
			for (int i = 0; i < 2; i++) {
				int x = i == 0 ? m_field.x + 1 : m_field.x - 1;
				Field f = Field(x, m_field.y - 1);
				if (state.occupied(f)) {
					fields.push_back(f);
				}
			}
		}
	}
	if (f1 && !state.occupied(forward1)) {
		fields.push_back(forward1);
	}
	if (f2 && !state.occupied(forward1) && !state.occupied(forward2)) {
		fields.push_back(forward2);
	}
	return fields;
}

Piece::Type Pawn::getType()
{
	return Type::PAWN;
}
