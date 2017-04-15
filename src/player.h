#ifndef CONCUDARING_PLAYER_H
#define CONCUDARING_PLAYER_H


#include "utils/Semaforo.h"
#include "deckOfCards.h"
#include "utils/SharedMemory.h"
#include "utils/types.h"
#include "utils/constants.h"

class Player {
private:
    const int id;
    Semaforo waitForACard;
    const int numberOfPlayers;
    Semaforo endOfTurnGathering;
    DeckOfCards myDeckOfCards;
    SharedMemory<Game_t> sharedMemory;

public:
    Player(int _id,int _numberOfPlayers);
    ~Player();
    void present() const ;

    void play();
    int increaseTurn(int turn);
    void setDeckOfCards(DeckOfCards& deck);
    bool itIsMyTurn(int turnNumber) const;
    bool checkWinner() const;

};


#endif //CONCUDARING_PLAYER_H
