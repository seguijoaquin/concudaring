#include <iostream>
#include "deckOfCards.h"



DeckOfCards::DeckOfCards() {}

DeckOfCards::~DeckOfCards() {}




void DeckOfCards::addCard(int card){
    cards.push_back(card);
}

DeckOfCards& DeckOfCards::operator+(DeckOfCards anotherDeck) {
    int card = anotherDeck.getCard();
    while (card != 0){
        addCard(card);
        card = anotherDeck.getCard();
    }
    return *this;
}


DeckOfCards& DeckOfCards::operator=(DeckOfCards anotherDeck) {
    std::cout << "Copy Assigment DeckOfCards" << "\n";
    std::swap(cards,anotherDeck.cards);
    return *this;
}

void DeckOfCards::print() {
    for (int i = 0; i < cards.size(); ++i) {
        std::cout << cards[i] << ".";
    }
    std::cout << "\n";
}

bool DeckOfCards::isEmpty() {
    return cards.empty();
}
int DeckOfCards::getCard() {
    int card =  cards.back();
    cards.pop_back();
    return card;
}