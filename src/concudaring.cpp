#include <iostream>
#include <unistd.h>
#include "concudaring.h"
#include "utils/Semaforo.h"
#include <sys/types.h>
#include <sys/wait.h>

Concudaring::Concudaring(int numberPlayers) {
    //TODO falta ver bien el creator, el valgrind tira error.
    configureCreator(numberPlayers);
    std::vector<DeckOfCards> decks = creator.getDeckOfCards();
    createPlayers(numberPlayers,decks);
    //TODO esto deberia crearse antes de crear a los players.
    char nombre[] = "/bin/cat";
    thereIsCard = Semaforo(nombre,0,'a');
    //std::cout << "Soy el concudaring con el process id:" << getpid() << std::endl;
}


void Concudaring::configureCreator(int numberPlayers){
    creator.mixCards(); //mezclo las cartas
    creator.setNumberOfPlayers(numberPlayers); //seteo cantidad de jugadores
}

void Concudaring::createPlayers(int numberPlayers, std::vector<DeckOfCards>& decks){

    //TODO: REFACTOR USANDO UN SOLO SEMAFORO QUE CREO QUE SE PUEDE
    //Podriamos inicializar todos los recursos aca.
    //Semaforo* waitForACard = new Semaforo(FILE_CONCUDARING,0, KEY_SEM_WAIT_FOR_A_CARD);
    //Semaforo* endOfTurnGathering = new Semaforo(FILE_CONCUDARING,0, KEY_SEM_END_OF_TURN_GATHERING);

    //std::cout << "El juego inicia" << std::endl;

    pid_t pid_padre = getpid();

    for (int i = 0; i < numberPlayers ; ++i) {
        //TODO: una mejor idea seria forkear dentro del player.play y que ahi devuelva el idProcess y con
        //hacer el wait, asi no ahorammos el exit y se destruye todo de manera correcta.

        pid_t pid = fork();
        if (pid == 0){
            DeckOfCards deck = decks[i];
            //Player player(i, waitForACard, endOfTurnGathering, numberPlayers);
            Player player(i,numberPlayers);
            player.setDeckOfCards(deck);
            player.play();
            return;
        }


    }


    //TODO: Este if no seria necesario no?
    if (pid_padre = getpid()) {
      std::cout << "Todos los procesos fuern lanzados\n";
      for (int j = 0; j <  numberPlayers; ++j) {
            wait(NULL);
          }
    //delete(endOfTurnGathering);
    //delete(waitForACard);
  }

}


void Concudaring::start(){

}

Concudaring::~Concudaring() {
  //std::cout << "Soy el concudaring con el process id (destroy):" << getpid() << std::endl;

  thereIsCard.eliminar();

}
