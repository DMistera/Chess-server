#include "game.h"

Game::Game(int white, int black) {
    m_blackSocket = black;
    m_whiteSocket = white;
}

int Game::getWhiteSocket() {
    return m_whiteSocket;
}

int Game::getBlackSocket() {
    return m_blackSocket;
}