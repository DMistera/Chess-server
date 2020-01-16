#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>
#include <iostream>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <memory>

#include "game.h"
#include "clientManager.h"

#define PORT 1234
#define QUEUE_SIZE 5

using namespace std;

// Data for individual client thread.
struct ClientThreadData
{
    int socket; // Client socket
    //Shared data:
    ClientManager* manager;
};

void *threadBehavior(void *t_data)
{
    pthread_detach(pthread_self());
    ClientThreadData *th_data = (ClientThreadData*)t_data;
    shared_ptr<Game> game;
    cout << "New socket:" << th_data->socket << endl;
    char* readBuf;
    while(1) {
        readBuf = new char[Consts::MESSAGE_SIZE];
        int readResult = read(th_data->socket, readBuf, sizeof(char) * Consts::MESSAGE_SIZE);
        if(readResult > 0 && readResult <= (signed int)Consts::MESSAGE_SIZE) {
            cout << "Message: " << readBuf << " from " << th_data->socket << endl;
                ClientManager* manager = th_data->manager;
                int socket = th_data->socket;
                char msgId = readBuf[0];
                switch (msgId)
                {
                case 'R':
                    // If there is no player in a lobby, wait in a lobby
                    if(manager->isLobbyEmpty()) {
                        // Put current player to the lobby.
                        manager->subscribe(socket, [&](int opponent, function<void(shared_ptr<Game>)> response) {
                            game = make_shared<Game>(socket, opponent);
                            response(game);
                        });
                        cout << "Waiting player is now " << socket << endl;
                    }
                    // if there is a player in a lobby, start a game with him
                    else {
                        manager->call(socket, [&](shared_ptr<Game> g) {
                            game = g;
                            g->start();
                        });
                    }
                    break;
                case 'C':
                    manager->unsubscribe(socket);
                    break;
                case 'M':
                    if(game) {
                        game->applyMove(socket, readBuf);
                    }
                    break;
                default:
                    break;
                }
        }
        else if(readResult <= -1) {
            if(game) {
                game->end();
            }
            else {
                th_data->manager->unsubscribe(th_data->socket);
            }
            cerr << "Error at reading: " << readResult << endl;
            break;
        }
        else {
            if(game) {
                game->end();
            }
            else {
                th_data->manager->unsubscribe(th_data->socket);
            }
            break;
        }
    }
    cout << "Ending connection for socket:" << th_data->socket << endl;
    delete[] readBuf;
    delete th_data;
    pthread_exit(NULL);
}

void signal_callback_handler(int signum){

    cout << "Caught signal SIGPIPE " << signum << endl;
}

int main() {

  signal(SIGPIPE, signal_callback_handler);

   char reuse_addr_val = 1;
   sockaddr_in server_address;
   //inicjalizacja gniazda serwera
   memset(&server_address, 0, sizeof(sockaddr));
   server_address.sin_family = AF_INET;
   server_address.sin_addr.s_addr = htonl(INADDR_ANY);
   server_address.sin_port = htons(PORT);

   int server_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
   if (server_socket_descriptor < 0)
   {
       cerr << "Błąd przy próbie utworzenia gniazda.." << endl;
       exit(1);
   }
   setsockopt(server_socket_descriptor, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse_addr_val, sizeof(reuse_addr_val));

   int bind_result = bind(server_socket_descriptor, (sockaddr*)&server_address, sizeof(sockaddr));
   if (bind_result < 0)
   {
       cerr << "Błąd przy próbie dowiązania adresu IP i numeru portu do gniazda." << endl;
       exit(1);
   }

   int listen_result = listen(server_socket_descriptor, QUEUE_SIZE);
   if (listen_result < 0) {
       cerr << "Błąd przy próbie ustawienia wielkości kolejki." << endl;
       exit(1);
   }

   cout << "Server is ready." << endl;

   ClientManager* clientManager = new ClientManager();

   while(1)
   {
        int connection_socket_descriptor = accept(server_socket_descriptor, NULL, NULL);
        if (connection_socket_descriptor < 0)
        {
            cerr << "Błąd przy próbie utworzenia gniazda dla połączenia." << endl;
            exit(1);
        }

        ClientThreadData* t_data = new ClientThreadData();
        t_data->socket = connection_socket_descriptor;
        t_data->manager = clientManager;

        pthread_t thread1;
        int create_result = pthread_create(&thread1, NULL, threadBehavior, (void *)t_data);
        if (create_result){
            cerr << "Błąd przy próbie utworzenia wątku, kod błędu: " << create_result << endl;
            exit(-1);
        }
   }

    cout << "End." << endl;
   close(server_socket_descriptor);
   delete clientManager;
   return(0);
}