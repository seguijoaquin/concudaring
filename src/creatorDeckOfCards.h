#ifndef CONCUDARING_CREATORDECKOFCARDS_H
#define CONCUDARING_CREATORDECKOFCARDS_H


#include <vector>
#include "deckOfCards.h"

class CreatorDeckOfCards {
    int* cards;
    int firstNumber,lastNumber,quantity;
    int quantityPlayers;
    int generateRandomPos() const;
    void print() const;
    void setNumbers(int _firstNumber,int _lastNumber);
    void createDeckOfCards();
public:
    CreatorDeckOfCards();
    void setNumberOfCards(int _quantity);
    ~CreatorDeckOfCards();
    void setNumberOfPlayers(int _quantityPlayers);
    void mixCards();
    std::vector<DeckOfCards> getDeckOfCards();
};


#endif //CONCUDARING_CREATORDECKOFCARDS_H
