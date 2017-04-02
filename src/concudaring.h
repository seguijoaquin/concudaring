#ifndef CONCUDARING_CONCUDARING_H
#define CONCUDARING_CONCUDARING_H


#include <vector>
#include "table.h"
#include "player.h"

class Concudaring {
    Table table;
    std::vector<Player> players;
    void createPlayers(int numberPlayers);

public:
    Concudaring(int numberPlayers);
    ~Concudaring();
    void start();
};


#endif //CONCUDARING_CONCUDARING_H
