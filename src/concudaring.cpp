#include <iostream>
#include <unistd.h>
#include "concudaring.h"
#include "utils/Semaforo.h"
#include <sys/types.h>
#include <sys/wait.h>

Concudaring::Concudaring(int numberPlayers) {
    configureCreator(numberPlayers);
    std::vector<DeckOfCards> decks = creator.getDeckOfCards();
    char nombre[] = "/bin/cat";
    thereIsCard = Semaforo(nombre,0,'a');
    createPlayers(numberPlayers,decks);
}


void Concudaring::configureCreator(int numberPlayers){
    creator.mixCards(); //mezclo las cartas
    creator.setNumberOfPlayers(numberPlayers); //seteo cantidad de jugadores
}

void Concudaring::createPlayers(int numberPlayers, std::vector<DeckOfCards>& decks){
    pid_t pid_padre = getpid();
    for (int i = 0; i < numberPlayers ; ++i) {
        pid_t pid = fork();
        if (pid == 0){
            DeckOfCards deck = decks[i];
            Player player(i,numberPlayers);
            player.setDeckOfCards(deck);
            player.play();
            return;
        }


    }

    //TODO: Este if no seria necesario no?
    if (pid_padre == getpid()) {
      std::cout << "Todos los procesos fuern lanzados\n";
      for (int j = 0; j <  numberPlayers; ++j) {
            wait(NULL);
          }
  }

}


void Concudaring::start(){

}

Concudaring::~Concudaring() {
  thereIsCard.eliminar();

}
