#include <iostream>
#include <unistd.h>
#include "concudaring.h"
#include "../playground/Semaforo.h"
#include <sys/types.h>
#include <sys/wait.h>

#define NOMBRE "/bin/ls"
#define NOMBRE2 "/bin/echo"

Concudaring::Concudaring(int numberPlayers) {
    createPlayers(numberPlayers);
    creator.setNumberOfCards(48);
    creator.createDeckOfCards();
    creator.setNumbers(1,12);
    creator.mixCards();
    creator.setNumberOfPlayers(numberPlayers);
    creator.getDeckOfCards();
}


void Concudaring::createPlayers(int numberPlayers){

    //TODO: REFACTOR USANDO UN SOLO SEMAFORO QUE CREO QUE SE PUEDE
    //Podriamos inicializar todos los recursos aca.
    Semaforo* waitForACard = new Semaforo((char*) NOMBRE,0);
    Semaforo* waitToSeeIfThereIsAWinner = new Semaforo((char*) NOMBRE2,0);

    for (int i = 0; i < numberPlayers ; ++i) {
        pid_t pid = fork();
        if (pid == 0){
            Player player(i, waitForACard, waitToSeeIfThereIsAWinner, numberPlayers);
            player.play();
            exit(0);
        }
    }

    std::cout << "El juego inicia" << std::endl;

    for (int j = 0; j <  numberPlayers; ++j) {
        wait(NULL);
    }

    waitForACard->eliminar();
    free(waitForACard);
    waitToSeeIfThereIsAWinner->eliminar();
    free(waitToSeeIfThereIsAWinner);

}


void Concudaring::start(){

}

Concudaring::~Concudaring() {}