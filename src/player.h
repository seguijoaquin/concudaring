#ifndef CONCUDARING_PLAYER_H
#define CONCUDARING_PLAYER_H


#include "deckOfCards.h"

class Player {
private:
    int id;
    DeckOfCards myDeckOfCards;
public:
    Player(int _id);
    ~Player();
    void present();
};


#endif //CONCUDARING_PLAYER_H
