#include <iostream>
#include <unistd.h>
#include "concudaring.h"
#include "utils/Semaforo.h"
#include <sys/types.h>
#include <sys/wait.h>

Concudaring::Concudaring(int numberPlayers) {
    createSemaphores(numberPlayers);
    createSharedMemories();
    configureCreator(numberPlayers);
    std::vector<DeckOfCards> decks = creator.getDeckOfCards();
    createPlayers(numberPlayers,decks);
    freeSemaphores();
}


void Concudaring::configureCreator(int numberPlayers){
    creator.mixCards(); //mezclo las cartas
    creator.setNumberOfPlayers(numberPlayers); //seteo cantidad de jugadores
}


void Concudaring::createPlayers(int numberPlayers, std::vector<DeckOfCards>& decks){
    std::vector<pid_t> childrenIds;
    pid_t pid_padre = getpid();
    pid_t pid = 0;
    for (int i = 0; i < numberPlayers ; ++i) {
        pid = fork(); //Creo un hijo
        if (pid == 0){
          //Si soy el hijo..
          DeckOfCards deck = decks[i];
          Player player(i,numberPlayers);
          player.setDeckOfCards(deck);
          player.play();
          exit(0); //Debo salir del for una vez que soy el hijo
        } else {
            childrenIds.push_back(pid);
        }
    }

    //Si soy el padre entonces espero a mis hijos y libero los recursos
    if (pid_padre == getpid()) {
      for (int j = 0; j <  numberPlayers; ++j) {
        waitpid(childrenIds[j],NULL,0);
      }
    }

}


void Concudaring::start(){

}

Concudaring::~Concudaring() {

}

void Concudaring::createSemaphores(int numberOfPlayers) {
    thereIsCard = Semaforo(FILE_CONCUDARING,KEY_SEM_THERE_IS_CARD);
    writeIdLoser = Semaforo(FILE_CONCUDARING,KEY_SEM_WRITE_LOSER);
    readIdLoser = Semaforo(FILE_CONCUDARING,KEY_SEM_READ_LOSER);
    writeNumberOfCards = Semaforo(FILE_CONCUDARING,KEY_SEM_WRITE_NUMBER_OF_CARDS);
    gatheringPoint = Semaforo(FILE_CONCUDARING,'g');
    writeIdLoser.inicializar(1);
    thereIsCard.inicializar(0);
    writeNumberOfCards.inicializar(1);
    readIdLoser.inicializar(numberOfPlayers);
    gatheringPoint.inicializar(numberOfPlayers);
}

void Concudaring::freeSemaphores() {
    thereIsCard.eliminar();
    writeIdLoser.eliminar();
    readIdLoser.eliminar();
    gatheringPoint.eliminar();
    writeNumberOfCards.eliminar();
}

void Concudaring::createSharedMemories(){
    SharedMemory<int> numberOfPlayersPutHand;
    SharedMemory<int> numberOfPlayerThatWrote;
    SharedMemory<int> i;
    //Inicializo la memoria de la mesa que cuenta la cantidad que jugadores que pusieron la mano
    numberOfPlayersPutHand.create(FILE_CONCUDARING,KEY_SHME_TABLE_NUMBER_PLAYER_PUT_HAND,1);
    numberOfPlayersPutHand.write(0);

    //Inicializo la memoria del juez que cuenta la cantidad de jugadores que le mandaron informacion
    numberOfPlayerThatWrote.create(FILE_CONCUDARING,KEY_SHME_JUDGE_NUMBER,1);
    numberOfPlayerThatWrote.write(0);

    //Inicializo la memoria de la mesa que cuenta cantidad de cartas que tiene
    i.create(FILE_CONCUDARING,KEY_SHME_TABLE_I,1);
    i.write(0);
}
