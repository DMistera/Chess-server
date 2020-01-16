#include "GameState.h"



GameState::GameState()
{
	m_pieces = std::list<Piece*>();

	//Set up a starting board
	//Pawns
	for (int i = 0; i < 8; i++) {
		Pawn* whitePawn = new Pawn(Side::WHITE, i);
		m_pieces.push_back(whitePawn);

		Pawn* blackPawn = new Pawn(Side::BLACK, i);
		m_pieces.push_back(blackPawn);
	}

	//Rest of the peices
	for (int i = 0; i < 2; i++) {
		Side side = i == 0 ? Side::WHITE : Side::BLACK;
		m_pieces.push_back(new King(side));
		m_pieces.push_back(new Queen(side));
		for (int x = 0; x < 8; x += 7) {
			m_pieces.push_back(new Rook(side, x));
		}
		for (int x = 1; x < 7; x += 5) {
			m_pieces.push_back(new Knight(side, x));
		}
		for (int x = 2; x < 6; x += 3) {
			m_pieces.push_back(new Bishop(side, x));
		}
	}

}


GameState::~GameState()
{
	//m_pieces.clear();
}

void GameState::applyMove(Move& move)
{
	Piece* target = getPieceAtField(move.getEndField());
	if (target) {
		m_pieces.remove(target);
	}
	Piece* p = getPieceAtField(move.getStartField());
	if (!p) {
		throw std::exception();
	}
	if(p->getSide() != m_activeSide) {
		throw std::exception();
	}
	p->setField(move.getEndField());
}

std::list<Piece*> GameState::getPieces() const
{
	return m_pieces;
}

Piece * GameState::getPieceAtField(Field field)
{
	for (std::list<Piece*>::iterator it = m_pieces.begin(); it != m_pieces.end(); ++it) {
		Piece* piece = (*it);
		if (piece->getField().x == field.x && piece->getField().y == field.y) {
			return piece;
		}
	}
	return nullptr;
}

King * GameState::getKing(Side side)
{
	for (Piece* p : m_pieces) {
		if (p->getSide() == side && p->getType() == Piece::Type::KING) {
			return dynamic_cast<King*>(p);
		}
	}
	return nullptr;
}

bool GameState::occupied(Field field)
{
	return getPieceAtField(field) != nullptr;
}

Side GameState::getActiveSide() const
{
	return m_activeSide;
}

void GameState::setActiveSide(Side side)
{
	m_activeSide = side;
}

bool GameState::checkMate()
{
	return getKing(Side::WHITE) == nullptr || getKing(Side::BLACK) == nullptr;
}
