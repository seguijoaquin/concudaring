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
    thereIsCard = Semaforo(nombre,'t');
    thereIsCard.inicializar(0);
    createPlayers(numberPlayers,decks);
}


void Concudaring::configureCreator(int numberPlayers){
    creator.mixCards(); //mezclo las cartas
    creator.setNumberOfPlayers(numberPlayers); //seteo cantidad de jugadores
}

void Concudaring::createPlayers(int numberPlayers, std::vector<DeckOfCards>& decks){

    //TODO: PUEDE IR EN OTRA FUNCION DE CREAR SEMAFOROS
    Semaforo endOfTurnGathering(FILE_CONCUDARING,KEY_SEM_END_OF_TURN_GATHERING);
    endOfTurnGathering.inicializar(0);
    char nombre[] = "/bin/cat";
    //Semaforo thereIsCard(nombre,'t');
    //thereIsCard.inicializar(0);
    Semaforo writeIdLosser(nombre,'w');
    writeIdLosser.inicializar(1);
    Semaforo readIdLosser(nombre,'r');
    readIdLosser.inicializar(0);


    for (int i = 0; i < numberPlayers ; ++i) {
        pid_t pid = fork();
        if (pid == 0){
            DeckOfCards deck = decks[i];
            Player player(i,numberPlayers);
            player.setDeckOfCards(deck);
            player.play();
            return;
        } else if ( pid == -1 ){
            //TODO: PRINTEAR FORK ERROR
        }
    }
    //std::cout << "SLKDJLFLSDKFJLDSKF" << std::endl;
    for (int j = 0; j <  numberPlayers; ++j) {
        wait(NULL);
    }

    std::cout << "eliminando semafotos\n" ;
    thereIsCard.eliminar();
    endOfTurnGathering.eliminar();
    writeIdLosser.eliminar();
    readIdLosser.eliminar();

}


void Concudaring::start(){

}

Concudaring::~Concudaring() {

}
