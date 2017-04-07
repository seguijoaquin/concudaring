#ifndef CONCUDARING_CONCUDARING_H
#define CONCUDARING_CONCUDARING_H


#include <vector>
#include "table.h"
#include "player.h"
#include "creatorDeckOfCards.h"

class Concudaring {
    Table table;
    std::vector<Player> players;
    CreatorDeckOfCards creator;
    void createPlayers(int numberPlayers,std::vector<DeckOfCards>& decks);
    void configureCreator(int numberPlayers);

public:
    Concudaring(int numberPlayers);
    ~Concudaring();
    void start();
};


#endif //CONCUDARING_CONCUDARING_H
