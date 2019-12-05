#include "Move.h"




Move::Move(Field startField, Field endField) {
	m_startField = startField;
	m_endField = endField;
}

Move::Move(char* msg)
{
	if (msg[0] == 'M') {
		int sx = msg[1] - '0';
		int sy = msg[2] - '0';
		int ex = msg[3] - '0';
		int ey = msg[4] - '0';
		m_startField = Field(sx, sy);
		m_endField = Field(ex, ey);
	}
	else {
		cerr << "Invalid move message: " << msg << endl;
		throw exception();
	}
}

Move::~Move() {
}

string Move::toString()
{
	std::stringstream ss;
	ss << "M" << m_startField.x << m_startField.y << m_endField.x << m_endField.y;
	return ss.str();
}

Field Move::getStartField()
{
	return m_startField;
}

Field Move::getEndField()
{
	return m_endField;
}
