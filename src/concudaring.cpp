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
    writeIdLoser.inicializar(1);
    thereIsCard.inicializar(0);
    readIdLoser.inicializar(numberOfPlayers);
}

void Concudaring::freeSemaphores() {
    thereIsCard.eliminar();
    writeIdLoser.eliminar();
    readIdLoser.eliminar();
}

void Concudaring::createSharedMemories() {
    SharedMemory<int> numberOfPlayersPutHand;
    numberOfPlayersPutHand.create(FILE_CONCUDARING,'n',1);
    std::cout << "numberOfPlayersPutHand shmID: ";
    numberOfPlayersPutHand.printID();
    int initialValue =0;
    numberOfPlayersPutHand.write(initialValue);
}
