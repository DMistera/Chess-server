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

void Game::start() {
    write(m_whiteSocket, "AW", sizeof(char)*Consts::MESSAGE_SIZE);
    write(m_blackSocket, "AB", sizeof(char)*Consts::MESSAGE_SIZE);
}