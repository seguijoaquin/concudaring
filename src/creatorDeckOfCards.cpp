#include <cstdlib>
#include "creatorDeckOfCards.h"



CreatorDeckOfCards::CreatorDeckOfCards() {}

void CreatorDeckOfCards::setNumberOfCards(int _quantity) {
    quantity = _quantity;
    cards = new int[quantity];
}

CreatorDeckOfCards::~CreatorDeckOfCards() {
    delete[] cards;
}


void CreatorDeckOfCards::setNumberOfPlayers(int _quantityPlayers){
    quantityPlayers = _quantityPlayers;
}

void CreatorDeckOfCards::createDeckOfCards() {
    for (int i = 0; i <= 4; ++i) {
        for (int j = firstNumber; j < lastNumber; ++j) {
            cards[i*lastNumber + j -1] = j;
        }
    }
}


void CreatorDeckOfCards::setNumbers(int _firstNumber,int _lastNumber){
    firstNumber = _firstNumber;
    lastNumber = _lastNumber;
}


void CreatorDeckOfCards::mixCards() {
    for (int k = 0; k < 30 ; ++k) {
        int i = generateRandomPos();
        int j = generateRandomPos();
        int aux = cards[i];
        cards[i] = cards[j];
        cards[j] = aux;
    }
}


int CreatorDeckOfCards::generateRandomPos() {
    return rand() % quantity;
}

std::vector<DeckOfCards> CreatorDeckOfCards::getDeckOfCards(){
    std::vector<DeckOfCards> myVector;
    return myVector;
}
