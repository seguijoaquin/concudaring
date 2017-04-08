#ifndef CONCUDARING_PLAYER_H
#define CONCUDARING_PLAYER_H


#include "Semaforo.h"
#include "deckOfCards.h"
#include "utils/MemoriaCompartida.h"
#include "utils/types.h"
#include "utils/constants.h"

class Player {
private:
    int id;
    Semaforo *waitForACard;
    int numberOfPlayers;
    Semaforo *waitToSeeIfThereIsAWinner;
    DeckOfCards myDeckOfCards;
    MemoriaCompartida<Game_t> sharedMemory;

public:
    Player(int _id, Semaforo *semaforo, Semaforo *numberOfPlayers, int i);
    ~Player();
    void present();

    void play();
    void setDeckOfCards(DeckOfCards deck);
    bool itIsMyTurn(int turnNumber);
    bool checkWinner() const;

    bool iHaveNoCardsInMyDeck();



};


#endif //CONCUDARING_PLAYER_H
