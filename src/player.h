#ifndef CONCUDARING_PLAYER_H
#define CONCUDARING_PLAYER_H


#include "../playground/Semaforo.h"

class Player {
private:
    int id;
    Semaforo *waitForACard;
    int numberOfPlayers;
    Semaforo *waitToSeeIfThereIsAWinner;
public:
    Player(int _id, Semaforo *semaforo, Semaforo *numberOfPlayers, int i);
    ~Player();
    void present();

    void play();

    bool itIsMyTurn(int turnNumber);

    bool iHaveNoCardsInMyDeck();



};


#endif //CONCUDARING_PLAYER_H
