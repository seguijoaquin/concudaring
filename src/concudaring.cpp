#include <iostream>
#include <unistd.h>
#include "concudaring.h"
#include "utils/Semaforo.h"
#include <sys/types.h>
#include <sys/wait.h>

Concudaring::Concudaring(int numberPlayers) {
    configureCreator(numberPlayers);
    std::vector<DeckOfCards> decks = creator.getDeckOfCards();
    thereIsCard = Semaforo(FILE_CONCUDARING,KEY_SEM_THERE_IS_CARD);

    createPlayers(numberPlayers,decks);
}


void Concudaring::configureCreator(int numberPlayers){
    creator.mixCards(); //mezclo las cartas
    creator.setNumberOfPlayers(numberPlayers); //seteo cantidad de jugadores
}

void Concudaring::createPlayers(int numberPlayers, std::vector<DeckOfCards>& decks){
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
          break; //Debo salir del for una vez que soy el hijo
        }
    }

    //Si soy el padre entonces espero a mis hijos y libero los recursos
    if (pid_padre == getpid()) {
      for (int j = 0; j <  numberPlayers; ++j) {
        wait(NULL);
      }
    }
}


void Concudaring::start(){

}

Concudaring::~Concudaring() {

}
