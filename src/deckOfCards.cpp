#include "deckOfCards.h"



DeckOfCards::DeckOfCards() {}

DeckOfCards::~DeckOfCards() {}


void DeckOfCards::addDeckOfCards(DeckOfCards &otherDeckOfCards) {
    int card = otherDeckOfCards.getCard();
    while (card != 0){
        addCard(card);
        card = otherDeckOfCards.getCard();
    }
}

void DeckOfCards::addCard(int card){
    cards.push_back(card);
}
int DeckOfCards::getCard() {
    int card =  cards.back();
    cards.pop_back();
    return card;
}