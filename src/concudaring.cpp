#include <iostream>
#include <unistd.h>
#include "concudaring.h"
#include <sys/types.h>
#include <sys/wait.h>

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
    for (int i = 0; i < numberPlayers ; ++i) {
        pid_t pid = fork();
        if (pid == 0){
            Player player(i);
            players.push_back(player); //esto no funciona
            exit(0);
        }
    }

    for (int j = 0; j <  numberPlayers; ++j) {
        wait(NULL);
    }
}


void Concudaring::start(){
    std::cout << "El juego inicia" << std::endl;
}
Concudaring::~Concudaring() {}