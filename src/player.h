#ifndef CONCUDARING_PLAYER_H
#define CONCUDARING_PLAYER_H


#include "utils/Semaforo.h"
#include "deckOfCards.h"
#include "utils/SharedMemory.h"
#include "utils/types.h"
#include "utils/constants.h"
#include "table.h"
#include "utils/logger/logger.h"
#include "utils/logger/mensajes.h"

class Player {
private:
    const int id;
    const int numberOfPlayers;
    Semaforo endOfTurnGathering;
    DeckOfCards myDeckOfCards;
    SharedMemory<bool> noOneWon;

public:
    Player(int _id,int _numberOfPlayers);
    ~Player();
    void present() const ;

    void play();
    int increaseTurn(int turn);
    void setDeckOfCards(DeckOfCards& deck);
    bool itIsMyTurn(int turnNumber) const;
    bool checkWinner() const;

    void checkCardsAndPerformAction();
};


#endif //CONCUDARING_PLAYER_H
