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
    writeToSocket(m_whiteSocket, "AW");
    writeToSocket(m_blackSocket, "AB");
}

void Game::end() {
    writeToSocket(m_whiteSocket, "D");
    writeToSocket(m_blackSocket, "D");
    cout << "Game ended!" << endl;
    m_ended = true;
}

void Game::applyMove(int socket, char* moveStr) {
    if(!m_ended) {
        int opponent;
        if(socket == m_whiteSocket) {
            m_state.setActiveSide(Side::WHITE);
            opponent = m_blackSocket;
        }
        else {
            m_state.setActiveSide(Side::BLACK);
            opponent = m_whiteSocket;
        }
        Move move = Move(moveStr);
        try {
            m_state.applyMove(move);
            cout << "Client " << socket << " made a move: " << moveStr << endl;
            writeToSocket(opponent, moveStr);
            if(m_state.checkMate()) {
                m_ended = true;
            }

        } catch(std::exception e) {
            cerr << "Client " << socket << " made a wrong move: " << moveStr << endl;
            end();
        }
    }
}

void Game::writeToSocket(int socket, const char* moveStr) {
    if(write(socket, moveStr, sizeof(char)*Consts::MESSAGE_SIZE) == - 1) {
        cerr << "Failed to write " << moveStr << " to socket" << socket << ". Ending game." << endl;
        m_ended = true;
    }
    else {
        cout << "Wrote " << moveStr << " to socket " << socket << endl;
    }
}