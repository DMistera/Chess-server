#pragma once

#include <map>
#include <functional>
#include "game.h"

using namespace std;

typedef function<void(int, function<void(Game*)>)> ReadyCallback;

class ClientManager {
    public:
        void subscribe(int socket, ReadyCallback);
        void unsubscribe(int socket);
        bool isLobbyEmpty();
        void call(int, function<void(Game*)>);
    private:
        map<int, ReadyCallback> m_queue;
};