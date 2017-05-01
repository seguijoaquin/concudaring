#include <cstdlib>
#include <iostream>
#include "creatorDeckOfCards.h"



CreatorDeckOfCards::CreatorDeckOfCards() {
    quantity = 48;
    cards = new int[quantity];
    setNumbers(1,12);
    createDeckOfCards();
}


CreatorDeckOfCards::~CreatorDeckOfCards() {
    delete[] cards;
}


void CreatorDeckOfCards::setNumberOfPlayers(int _quantityPlayers){
    quantityPlayers = _quantityPlayers;
}

void CreatorDeckOfCards::createDeckOfCards() {
    for (int i = 0; i < 4; ++i) {
        for (int j = firstNumber; j <= lastNumber; ++j) {
            cards[i*lastNumber + j -1] = j;
        }
    }

}


void CreatorDeckOfCards::setNumbers(int _firstNumber,int _lastNumber){
    firstNumber = _firstNumber;
    lastNumber = _lastNumber;
}


void CreatorDeckOfCards::mixCards() {
    for (int k = 0; k < 40 ; ++k) {
        int i = generateRandomPos();
        int j = generateRandomPos();
        int aux = cards[i];
        int aux2 = cards[j];
        cards[i] = aux2;
        cards[j] = aux;
    }
}


void CreatorDeckOfCards::print(){
    for (int i = 0; i < quantity; ++i) {
        std::cout << cards[i] << ".";
    }
}

int CreatorDeckOfCards::generateRandomPos() {
    return rand() % quantity-1;
}

std::vector<DeckOfCards> CreatorDeckOfCards::getDeckOfCards(){
    std::vector<DeckOfCards> myVector;
    for (int k = 0; k < quantityPlayers; ++k) {
        DeckOfCards deck;
        myVector.push_back(deck);
    }

    for (int i = 0; i < quantity ; ++i) {
        int pos = i % quantityPlayers;
        DeckOfCards& deck = myVector[pos];
        deck.addCard(cards[i]);
    }
    return myVector;
}
