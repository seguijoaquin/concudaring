#include <iostream>
#include <unistd.h>
#include "concudaring.h"
#include "utils/Semaforo.h"
#include <sys/types.h>
#include <sys/wait.h>

Concudaring::Concudaring(int numberPlayers) {
    //TODO falta ver bien el creator, el valgrind tira error.
    //configureCreator(numberPlayers);
    //std::vector<DeckOfCards> decks = creator.getDeckOfCards();
    createPlayers(numberPlayers);
}


void Concudaring::configureCreator(int numberPlayers){
    creator.setNumberOfCards(48); //seteo cantidad de cartas
    creator.setNumbers(1,12); //seteo numeros posibles
    creator.createDeckOfCards(); // creo el mazo
    creator.mixCards(); //mezclo las cartas
    creator.setNumberOfPlayers(numberPlayers); //seteo cantidad de jugadores
}

void Concudaring::createPlayers(int numberPlayers){

    //TODO: REFACTOR USANDO UN SOLO SEMAFORO QUE CREO QUE SE PUEDE
    //Podriamos inicializar todos los recursos aca.
    Semaforo* waitForACard = new Semaforo(FILE_CONCUDARING,0, KEY_SEM_WAIT_FOR_A_CARD);
    Semaforo* endOfTurnGathering = new Semaforo(FILE_CONCUDARING,0, KEY_SEM_END_OF_TURN_GATHERING);

    for (int i = 0; i < numberPlayers ; ++i) {
        pid_t pid = fork();
        if (pid == 0){
            //DeckOfCards deck = decks[i];
            //player.setDeckOfCards(deck);
            Player player(i, waitForACard, endOfTurnGathering, numberPlayers);
            //DeckOfCards deck = decks[i];
            //player.setDeckOfCards(deck);
            player.play();
            exit(0);
        }
    }

    std::cout << "El juego inicia" << std::endl;

    for (int j = 0; j <  numberPlayers; ++j) {
        wait(NULL);
    }

    delete(endOfTurnGathering);
    delete(waitForACard);

}


void Concudaring::start(){

}

Concudaring::~Concudaring() {}