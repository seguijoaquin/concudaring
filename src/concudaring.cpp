#include <iostream>
#include <unistd.h>
#include "concudaring.h"
#include "utils/Semaforo.h"
#include <sys/types.h>
#include <sys/wait.h>

Concudaring::Concudaring(int numberPlayers) {
    configureCreator(numberPlayers);
    std::vector<DeckOfCards> decks = creator.getDeckOfCards();

    createPlayers(numberPlayers,decks);
}


void Concudaring::configureCreator(int numberPlayers){
    creator.mixCards(); //mezclo las cartas
    creator.setNumberOfPlayers(numberPlayers); //seteo cantidad de jugadores
}


void Concudaring::createPlayers(int numberPlayers, std::vector<DeckOfCards>& decks){

    //TODO: PUEDE IR EN OTRA FUNCION DE CREAR SEMAFOROS
    Semaforo endOfTurnGathering(FILE_CONCUDARING,KEY_SEM_END_OF_TURN_GATHERING);
    endOfTurnGathering.inicializar(numberPlayers - 1);
    thereIsCard = Semaforo(FILE_CONCUDARING,KEY_SEM_THERE_IS_CARD);
    thereIsCard.inicializar(0);
    Semaforo writeIdLosser(FILE_CONCUDARING,KEY_SEM_WRITE_LOSER);
    writeIdLosser.inicializar(1);
    Semaforo readIdLosser(FILE_CONCUDARING,KEY_SEM_READ_LOSER);
    readIdLosser.inicializar(numberPlayers);
    SharedMemory<int> numberOfPlayersPutHand;
    numberOfPlayersPutHand.create(FILE_CONCUDARING,'n',1);
    int initialValue =0;
    numberOfPlayersPutHand.write(initialValue);

    std::vector<pid_t> childIds;
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
            childIds.push_back(pid);
        }
    }

    //Si soy el padre entonces espero a mis hijos y libero los recursos
    if (pid_padre == getpid()) {
      for (int j = 0; j <  numberPlayers; ++j) {
        waitpid(childIds[j],NULL,0);
      }
      std::cout << "eliminando semafotos\n" ;
      thereIsCard.eliminar();
      endOfTurnGathering.eliminar();
      writeIdLosser.eliminar();
      readIdLosser.eliminar();
    }
}


void Concudaring::start(){

}

Concudaring::~Concudaring() {

}
