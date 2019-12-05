#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "consts.h"
#include <iostream>
#include "GameState.h"

using namespace std;

class Game {
    public:
        Game(int white, int black);
        int getWhiteSocket();
        int getBlackSocket();
        void start();
        void end();
        void applyMove(int socket, char* moveStr);
    private:
        void writeToSocket(int socket, const char* moveStr);
        int m_whiteSocket;
        int m_blackSocket;
        int m_ended;
        GameState m_state;
};