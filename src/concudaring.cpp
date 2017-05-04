#include <iostream>
#include <unistd.h>
#include "concudaring.h"
#include "utils/Semaforo.h"
#include <sys/types.h>
#include <sys/wait.h>
#include "judge.h"

Concudaring::Concudaring(int numberPlayers, double judgeTimeBetweenChecks) {
    createSemaphores(numberPlayers);
    createSharedMemories();
    configureCreator(numberPlayers);
    std::vector<DeckOfCards> decks = creator.getDeckOfCards();
    createPlayers(numberPlayers,decks,judgeTimeBetweenChecks);
    freeSemaphores();
}


void Concudaring::configureCreator(int numberPlayers){
    creator.mixCards(); //mezclo las cartas
    creator.setNumberOfPlayers(numberPlayers); //seteo cantidad de jugadores
}


void Concudaring::throwJudge(int numberPlayers,float timeBetweenChecks){
  Judge& judge = Judge::getInstance();
    judge.setTimeBetweenChecks(timeBetweenChecks);
  judge.start();
}

void Concudaring::stopJudge(){
  Judge& judge = Judge::getInstance();
  judge.stop();
}

void Concudaring::createPlayers(int numberPlayers, std::vector<DeckOfCards>& decks, double judgeTimeBetweenChecks){
    std::vector<pid_t> childrenIds;
    pid_t pid_padre = getpid();
    pid_t pid = 0;
    Judge::getInstance().setNumberOfPlayers(numberPlayers);
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
      throwJudge(numberPlayers,judgeTimeBetweenChecks);

      for (int j = 0; j <  numberPlayers; ++j) {
        waitpid(childrenIds[j],NULL,0);
      }
      stopJudge();
    }
}


void Concudaring::start(){

}

Concudaring::~Concudaring() {

}

void Concudaring::createSemaphores(int numberOfPlayers) {
    //Creacion de semaforos
    thereIsCard = Semaforo(FILE_CONCUDARING,KEY_SEM_THERE_IS_CARD);
    writeIdLoser = Semaforo(FILE_CONCUDARING,KEY_SEM_WRITE_LOSER);
    writeNumberOfCards = Semaforo(FILE_CONCUDARING,KEY_SEM_WRITE_NUMBER_OF_CARDS);
    gatheringPoint = Semaforo(FILE_CONCUDARING,'g');
    conditionSem = Semaforo(FILE_CONCUDARING,KEY_SEM_JUDE_CONDITION);

    //Inicializacion de los semaforos
    writeIdLoser.inicializar(1);
    thereIsCard.inicializar(0);
    gatheringPoint.inicializar(numberOfPlayers);
    writeNumberOfCards.inicializar(1);
    conditionSem.inicializar(1);
}

void Concudaring::freeSemaphores() {
    thereIsCard.eliminar();
    writeIdLoser.eliminar();
    gatheringPoint.eliminar();
    writeNumberOfCards.eliminar();
    conditionSem.eliminar();
}

void Concudaring::createSharedMemories(){
    //Inicializo la memoria de la mesa que cuenta la cantidad que jugadores que pusieron la mano
    numberOfPlayersPutHand.create(FILE_CONCUDARING,KEY_SHME_TABLE_NUMBER_PLAYER_PUT_HAND,1);
    numberOfPlayersPutHand.write(0);

    //Inicializo la memoria de la mesa que cuenta cantidad de cartas que tiene
    i.create(FILE_CONCUDARING,KEY_SHME_TABLE_I,1);
    i.write(0);

    //Inicializo la memoria compartida que usa el juez para saber hasta cuando tiene que imprimir
    condition.create(FILE_CONCUDARING,KEY_SHME_JUDGE_CONDITION,1);
    bool valor = true;
    condition.write(valor);
}
