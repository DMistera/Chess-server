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

#include "game.h"

#define PORT 1234
#define MESSAGE_SIZE 5
#define QUEUE_SIZE 5

using namespace std;

// Data for individual client thread.
struct ClientThreadData
{
    int socket; // Client socket
    //Shared data:
    int* waitingPlayer; // Pointer to socked id of the client waiting in a lobby
    Game** tmpGame; // Temporary pointer to game object to pass game reference between clients
    sem_t* waitingSemaphore; // Semaphore for watinging in a lobby.
};

struct MessageHandlerThreadData
{
    ClientThreadData* clientData;
    char* message;
    Game** game;
    bool* ready;
};

void semWait(sem_t* sem) {
    if(sem_wait(sem) == -1) {
        cerr << "Semaphore wait failed " << endl;
    }
}

void semPost(sem_t* sem) {
    if(sem_post(sem) == -1) {
        cerr << "Semaphore post failed " << endl;
    }
}

void cancelWait(bool* flag, sem_t* sem) {
    (*flag) = false;
    semPost(sem);
}

void* messageHandler(void *t_data) {
    pthread_detach(pthread_self());

    MessageHandlerThreadData *data = (MessageHandlerThreadData*)t_data;
    char* message = data->message;
    Game** gamePtr = data->game;
    bool* ready = data->ready;
    char msgId = message[0];
    int* waitingPlayer = data->clientData->waitingPlayer;
    Game** tmpGame = data->clientData->tmpGame;
    sem_t* semaphore = data->clientData->waitingSemaphore;
    switch (msgId)
    {
    case 'R':
        // If there is no player in a lobby, wait in a lobby
        if(*(waitingPlayer) == 0) {
            (*ready) = true;
            // Put current player to the lobby.
            *(waitingPlayer) = data->clientData->socket;
            cout << "Waiting player is now " << *(waitingPlayer) << endl;
            //Wait for another player
            semWait(semaphore);
            // If a player has cancelled watiing, the ready flag will be false;
            if(!*ready) {
                *(waitingPlayer) = 0;
                cout << "Player has canceled waiting: " << data->clientData->socket << endl;
            }
            else {
                // Check if game got passed correctly for safety.
                if(*tmpGame != nullptr) {
                    *tmpGame = nullptr;
                    *(gamePtr) = *(tmpGame);
                    write((*(gamePtr))->getWhiteSocket(), "A", sizeof(char) * MESSAGE_SIZE);
                }
                else {
                    cerr << "Game is not ready! (This should not happen)" << endl;
                }
            }
        }
        // if there is a player in a lobby, start a game with him
        else {
            int opponent = *(waitingPlayer);
            cout << "Opponent found: " << opponent << endl;
            *(waitingPlayer) = 0;
            *(gamePtr) = new Game(data->clientData->socket, opponent);
            *(tmpGame) = *(data->game);
            semPost(semaphore);
            write(opponent, "A", sizeof(char) * MESSAGE_SIZE);
        }
        break;
    case 'M':
        //TODO
    default:
        break;
    }
}

void *threadBehavior(void *t_data)
{
    pthread_detach(pthread_self());
    ClientThreadData *th_data = (ClientThreadData*)t_data;
    Game* game = nullptr;
    cout << "New socket:" << th_data->socket << endl;
    bool ready = false;
    while(1) {
        char* readBuf = new char[MESSAGE_SIZE];
        int readResult = read(th_data->socket, readBuf, sizeof(char) * MESSAGE_SIZE);
        if(readResult > 0) {
            cout << "Message: " << readBuf << " from " << th_data->socket << endl;
            MessageHandlerThreadData* msgData = new MessageHandlerThreadData();
            msgData->clientData = th_data;
            msgData->message = readBuf;
            msgData->game = &game;
            msgData->ready = &ready;
            pthread_t thread1;
            int create_result = pthread_create(&thread1, NULL, messageHandler, (void *)msgData);
            if (create_result){
                cerr << "Błąd przy próbie utworzenia wątku, kod błędu: " << create_result << endl;
                exit(-1);
            }
        }
        else if(readResult == 0) {
            cancelWait(&ready, th_data->waitingSemaphore);
            break;
        }
        else if(readResult < -1) {
            cancelWait(&ready, th_data->waitingSemaphore);
            cerr << "Error at reading: " << readResult << endl;
            break;
        }
    }
    cout << "Ending connection for socket:" << th_data->socket << endl;
    pthread_exit(NULL);
}



int main() {
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

   int waitingPlayer = 0;
   Game* activeGame = nullptr;
   sem_t waitingSemaphore;
   if(sem_init(&waitingSemaphore, 0, 0) == -1) {
        cerr << "Semaphore init error!" << endl;
        exit(1);
   }

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
        t_data->waitingPlayer = &waitingPlayer;
        t_data->tmpGame = &activeGame;
        t_data->waitingSemaphore = &waitingSemaphore;

        pthread_t thread1;
        int create_result = pthread_create(&thread1, NULL, threadBehavior, (void *)t_data);
        if (create_result){
            cerr << "Błąd przy próbie utworzenia wątku, kod błędu: " << create_result << endl;
            exit(-1);
        }
   }

   close(server_socket_descriptor);
   return(0);
}