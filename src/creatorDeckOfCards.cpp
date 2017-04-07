#include <cstdlib>
#include <iostream>
#include "creatorDeckOfCards.h"



CreatorDeckOfCards::CreatorDeckOfCards() {}


//quantity es la cantidad total de cartas en el mazo
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
        cards[i] = cards[j];
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


    //TODO: este loop se debe borrar
    std::cout << "Imprimiendo los masos de los jugadores" << std::endl;
    for (int j = 0; j < myVector.size(); ++j) {
        std::cout << "Mazo "<< j <<std::endl;
        DeckOfCards deck = myVector[j];
        deck.print();
    }
    return myVector;
}
