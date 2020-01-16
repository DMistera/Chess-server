#pragma once

#include <map>
#include <functional>
#include "game.h"
#include <memory>

using namespace std;

typedef function<void(int, function<void(shared_ptr<Game>)>)> ReadyCallback;

class ClientManager {
    public:
        void subscribe(int socket, ReadyCallback);
        void unsubscribe(int socket);
        bool isLobbyEmpty();
        void call(int, function<void(shared_ptr<Game>)>);
    private:
        map<int, ReadyCallback> m_queue;
};