#ifndef CONCUDARING_PLAYER_H
#define CONCUDARING_PLAYER_H


#include "Semaforo.h"
#include "deckOfCards.h"

class Player {
private:
    int id;
    Semaforo *waitForACard;
    int numberOfPlayers;
    Semaforo *endOfTurnGathering;
    DeckOfCards myDeckOfCards;
public:
    Player(int _id, Semaforo *semaforo, Semaforo *numberOfPlayers, int i);
    ~Player();
    void present();

    void play();
    void setDeckOfCards(DeckOfCards deck);
    bool itIsMyTurn(int turnNumber);

    bool iHaveNoCardsInMyDeck();
    void func();



};


#endif //CONCUDARING_PLAYER_H
