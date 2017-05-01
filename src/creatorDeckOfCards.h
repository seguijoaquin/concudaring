#ifndef CONCUDARING_CREATORDECKOFCARDS_H
#define CONCUDARING_CREATORDECKOFCARDS_H


#include <vector>
#include "deckOfCards.h"

class CreatorDeckOfCards {
    int* cards;
    int firstNumber,lastNumber,quantity;
    int quantityPlayers;
    int generateRandomPos();
public:
    CreatorDeckOfCards();
    void setNumberOfCards(int _quantity);
    ~CreatorDeckOfCards();
    void setNumbers(int _firstNumber,int _lastNumber);
    void createDeckOfCards();
    void mixCards();
    void print();
    std::vector<DeckOfCards> getDeckOfCards();
    void setNumberOfPlayers(int _quantityPlayers);
};


#endif //CONCUDARING_CREATORDECKOFCARDS_H
