#ifndef CONCUDARING_CONCUDARING_H
#define CONCUDARING_CONCUDARING_H


#include <vector>
#include "table.h"
#include "player.h"
#include "creatorDeckOfCards.h"

class Concudaring {
    CreatorDeckOfCards creator;
    void createPlayers(int numberPlayers,std::vector<DeckOfCards>& decks, double judgeTimeBetweenChecks);
    void configureCreator(int numberPlayers);
    Semaforo thereIsCard;
    Semaforo readIdLoser;
    Semaforo writeIdLoser;
    Semaforo gatheringPoint;
    Semaforo writeNumberOfCards;
    Semaforo conditionSem;


    SharedMemory<int> numberOfPlayersPutHand;
    SharedMemory<int> i;
    SharedMemory<bool> condition;

    void createSemaphores(int numberOfPlayers);
    void freeSemaphores();
    void createSharedMemories();
    void throwJudge(int numberPlayers, float timeBetweenChecks);
    void stopJudge();
public:
    Concudaring(int numberPlayers,double judgeTimeBetweenChecks);
    ~Concudaring();
    void start();
};


#endif //CONCUDARING_CONCUDARING_H
