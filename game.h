#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "consts.h"

class Game {
    public:
        Game(int white, int black);
        int getWhiteSocket();
        int getBlackSocket();
        void start();
    private:
        int m_whiteSocket;
        int m_blackSocket;
};