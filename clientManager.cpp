#include "clientManager.h"

void ClientManager::subscribe(int socket, ReadyCallback callback) {
    m_queue.insert(pair<int, ReadyCallback>(socket, callback));
}

void ClientManager::unsubscribe(int socket) {
    m_queue.erase(socket);
}

bool ClientManager::isLobbyEmpty() {
    return m_queue.size() == 0;
}

void ClientManager::call(int socket, function<void(Game*)> f) {
    int key = m_queue.begin()->first;
    m_queue.at(key)(socket, f);
    m_queue.erase(key);
}
